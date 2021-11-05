#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace net
{
	template<typename T>
	class server_interface
	{
	public:
		// creating a server that listens to the specified port
		server_interface(uint16_t port) :
			m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
		{
		}

		virtual ~server_interface()
		{
			// cleanup
			Stop();
		}

		// starts the server
		bool Start()
		{
			try
			{
				WaitForClientConnection();
				m_threadContext = std::thread([this]() { m_asioContext.run(); });
			}
			catch (std::exception& e)
			{
				// something prohibited the server from listening
				std::cerr << "[SERVER] EXCEPTION: " << e.what() << std::endl;
				return false;
			}

			std::cout << "[SERVER] started!\n";
			return true;
		}

		// stops the server
		void Stop()
		{
			// request the context to close
			m_asioContext.stop();

			// tidy up the context thread
			if (m_threadContext.joinable())	m_threadContext.join();

			// inform someone, anybody, if they care..
			std::cout << "[SERVER] Stopped!" << std::endl;
		}

		// async - instuct asio to wait for connection
		void WaitForClientConnection()
		{
			m_asioAcceptor.async_accept(
				[this](std::error_code ec, asio::ip::tcp::socket socket)
				{
					if (!ec)
					{
						std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << std::endl;

						std::shared_ptr<connection<T>> newconn =
							std::make_shared<connection<T>>(connection<T>::owner::Server,
								m_asioContext, std::move(socket), m_qMessagesIn);

						// give the user server a chance to deny connection
						if (OnClientConnect(newconn))
						{
							// connection allowed, so add to container of new connections
							m_deqConnections.push_back(std::move(newconn));

							m_deqConnections.back()->ConnectToClient(nIDCounter++);

							std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved" << std::endl;
						}
						else
						{
							std::cout << "[-----] Connection Denied" << std::endl;
						}
					}
					else
					{
						// error has occurred during acceptance
						std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
					}

					WaitForClientConnection();
				}
			);
		}

		void MessageClient(std::shared_ptr<connection<T>> client, const message<T>& msg)
		{
			if (client && client->IsConnected())
			{
				client->SendMsg(msg);
			}
			else
			{
				OnClientDisconnect(client);
				client.reset();

				m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
			}
		}

		void MessageAllClients(const message<T>& msg, std::shared_ptr<connection<T>> pIgnoreClient = nullptr)
		{
			bool bInvalidClientExists = false;

			for (auto& client : m_deqConnections)
			{
				// check client is connected...
				if (client && client->IsConnected())
				{
					// ..it is!
					if (client != pIgnoreClient)
					{
						client->SendMsg(msg);
					}
				}
				else
				{
					// the client couldnt be contacted, so assume it has
					// disconnected.
					OnClientDisconnect(client);
					client.reset();

					bInvalidClientExists = true;
				}
			}

			if (bInvalidClientExists)
			{
				m_deqConnections.erase(std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
			}
		}

		void Update(uint64_t nMaxMessages = -1, bool bwait = false)
		{
			if (bwait) m_qMessagesIn.wait();

			uint64_t nMessageCount = 0;
			while (nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
			{
				// grab the front message
				auto msg = m_qMessagesIn.pop_front();

				// pass the message handler
				OnMessage(msg.remote, msg.msg);

				nMessageCount++;
			}
		}

	protected:
		// called when a client connects, you can veto the connection by returning false
		virtual bool OnClientConnect(std::shared_ptr<connection<T>> client)
		{
			return false;
		}

		// called when a client appears to have disconnected
		virtual void OnClientDisconnect(std::shared_ptr<connection<T>> client)
		{
		}

		// when a message arrives
		virtual void OnMessage(std::shared_ptr<connection<T>> client, message<T>& msg)
		{
		}

	protected:
		// thread safe queue for incomming message packets
		tsqueue<owned_message<T>> m_qMessagesIn;

		// container of active validated connections
		std::deque<std::shared_ptr<connection<T>>> m_deqConnections;

		// order of declaration is important - it is also the order of initialisation
		asio::io_context m_asioContext;
		std::thread m_threadContext;

		// these things need an asio context
		asio::ip::tcp::acceptor m_asioAcceptor;

		//clients will be identified in the "wider system" via an ID
		uint32_t nIDCounter{ 100 };
	};
}