#pragma once

#include "net_common.h"

namespace net
{
	// packet header is sent at start of all packets. the template allows
	// to use "enum class" to ensure that the packet is valid at compile time
	template<typename T>
	struct message_header
	{
		T id;
		uint32_t size = 0;
	};

	template<typename T>
	struct message
	{
		message_header<T> header{};
		std::vector<uint8_t> body;

		size_t size() const
		{
			return body.size();
		}

		// override for std::cout compatibility - produces friendly description of packet
		friend std::ostream& operator << (std::ostream& os, const message<T> msg)
		{
			os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
			return os;
		}

		// pushes any POD-type into the buffer
		template<typename DataType>
		friend message<T>& operator << (message<T>& msg, const DataType& data)
		{
			// check that the type of data being pushed is trivially copyable
			static_assert(std::is_standard_layout<DataType>::value, "Data type is not a default type");

			// cash current size
			size_t i = msg.body.size();

			// resizing the vector
			msg.body.resize(msg.body.size() + sizeof(DataType));

			// manually copying the data
			std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

			// recalculate the message size
			msg.header.size = msg.size();

			// returning the message so it can be chained
			return msg;
		}

		// pulls any POD-type out of the message
		template<typename DataType>
		friend message<T>& operator >> (message<T>& msg, DataType& data)
		{
			// check that the type of data being pushed is trivially copyable
			static_assert(std::is_standard_layout<DataType>::value, "Data type is not a default type");

			// cash current size
			size_t i = msg.body.size() - sizeof(DataType);

			// manually copying the data
			std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

			// shrinking the vector
			msg.body.resize(i);

			// recalculate the message size
			msg.header.size = msg.size();

			// returning the message so it can be chained
			return msg;
		}
	};

	// forward declaring the connection
	template<typename T>
	class connection;

	template<typename T>
	struct owned_message
	{
		std::shared_ptr<connection<T>> remote = nullptr;
		message<T> msg{};

		friend std::ostream& operator << (std::ostream& os, const owned_message<T>& omsg)
		{
			os << omsg.msg;
			return os;
		}
	};
}