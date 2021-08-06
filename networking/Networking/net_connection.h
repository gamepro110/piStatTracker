#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace net
{
	template<typename T>
	class connection : public std::enable_shared_from_this<connection<T>>
	{
	public:
		enum class owner
		{
			Server,
			Client,
		};

		connection(owner parent, asio::io_context& asioContext, asio::ip::tcp::socket _socket, tsqueue<owned_message<T>>& qIn)
			: m_asioContext(asioContext), m_socket(std::move(_socket)), m_qMessagesIn(qIn)
		{
			m_nOwnerType = parent;
		}

		virtual ~connection()
		{}

		uint32_t GetID() const
		{
			return id;
		}

	public:
		void ConnectToClient(uint32_t uid = 0)
		{
			if (m_nOwnerType == owner::Server)
			{
				if (m_socket.is_open())
				{
					id = uid;
					ReadHeader();
				}
			}
		}

		void ConnectToServer(const asio::ip::tcp::resolver::results_type& endpoints)
		{
			// only clients can connect to servers
			if (m_nOwnerType == owner::Client)
			{
				// request asio attempts to connect to an endpoint
				asio::async_connect(m_socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
					{
						if (!ec)
						{
							ReadHeader();
						}
					}
				);
			}
		}

		void Disconnect()
		{
			if (IsConnected())
			{
				asio::post(m_asioContext, [this]() { m_socket.close(); });
			}
		}

		bool IsConnected() const
		{
			return m_socket.is_open();
		}

	public:
		void SendMsg(const message<T>& msg)
		{
			asio::post(m_asioContext,
				[this, msg]()
				{
					bool bWritingMessage = !m_qMessagesOut.empty();
					m_qMessagesOut.push_back(msg);
					if (!bWritingMessage)
					{
						WriteHeader();
					}
				}
			);
		}

	private:
		void WriteHeader()
		{
			asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						if (m_qMessagesOut.front().body.size() > 0)
						{
							WriteBody();
						}
						else
						{
							m_qMessagesOut.pop_front();

							if (!m_qMessagesOut.empty())
							{
								WriteHeader();
							}
						}
					}
					else
					{
						std::cout << "[" << id << "] Write Header Fail.\n";
						m_socket.close();
					}
				}
			);
		}

		void WriteBody()
		{
			asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body.data(), m_qMessagesOut.front().body.size()),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						m_qMessagesOut.pop_front();

						if (!m_qMessagesOut.empty())
						{
							WriteHeader();
						}
					}
					else
					{
						std::cout << "[" << id << "] Write Body Fail.\n";
						m_socket.close();
					}
				}
			);
		}

		// ASYNC - prime context ready to read a message header
		void ReadHeader()
		{
			asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.header, sizeof(message_header<T>)),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						if (m_msgTemporaryIn.header.size > 0)
						{
							m_msgTemporaryIn.body.resize(m_msgTemporaryIn.header.size);
							ReadBody();
						}
						else
						{
							AddToIncomingMessageQueue();
						}
					}
					else
					{
						std::cout << "[" << id << "] Read Header Fail.\n";
						m_socket.close();
					}
				}
			);
		}

		// ASYNC - prime context ready to read a message body
		void ReadBody()
		{
			asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.body.data(), m_msgTemporaryIn.body.size()),
				[this](std::error_code ec, size_t length)
				{
					if (!ec)
					{
						AddToIncomingMessageQueue();
					}
					else
					{
						std::cout << "[" << id << "] Read Body Fail.\n";
						m_socket.close();
					}
				}
			);
		}

		void AddToIncomingMessageQueue()
		{
			if (m_nOwnerType == owner::Server)
			{
				m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });
			}
			else
			{
				m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });
			}

			ReadHeader();
		}

	protected:
		// each connection has a unique socket to a remote
		asio::ip::tcp::socket m_socket;

		// this context is shared with the whole asio instance
		asio::io_context& m_asioContext;

		// this queue holds all messages to be sent to the remote side
		// of this connection
		tsqueue<message<T>> m_qMessagesOut;

		// this queue holds all messages that have been recieved from
		// the remote side of this connection. note it is a reference
		// as the "owner" of this connection is exxpected to provide a queue
		tsqueue<owned_message<T>>& m_qMessagesIn;
		message<T> m_msgTemporaryIn;

		// the "owner" decides how some of the connection behaves
		owner m_nOwnerType = owner::Server;
		uint32_t id = 0;
	};
}