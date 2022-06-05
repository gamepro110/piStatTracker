#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_client.h"
#include "net_server.h"
#include "net_connection.h"

namespace net
{
	// default list of message types. extend it where needed
	enum class MessageType : uint32_t
	{
		ServerAccept,
		ServerDeny,
		ServerPing,
		ServerSendStats,
	};

	/// <summary>preps string to be inserted into the message.</summary>
	/// <typeparam name="T">template for the message</typeparam>
	/// <param name="msg">the message that gets send</param>
	/// <param name="str">the string u want to send</param>
	template<typename T>
	void MsgStringWrapper(message<T>& msg, const std::string& str)
	{
		// TODO insert string backwards into the message
		uint64_t length{ std::strlen(str.c_str()) };

		for (int i = length - 1; i >= 0; i--)
		{
			msg << str[i];
		}

		msg << length;
	}

	/// <summary>preps string to be retrieved from the message.</summary>
	/// <typeparam name="T">template for the message</typeparam>
	/// <param name="msg">the recieved message</param>
	/// <param name="str">the string object that the retrieved string gets saved in</param>
	template<typename T>
	void MsgStringUnwrapper(message<T>& msg, std::string& str)
	{
		uint64_t length;
		msg >> length;

		for (int i = 0; i < length; i++)
		{
			char item{};
			msg >> item;
			str += item;
		}
	}
}

// IMPORTANT!!!
//
// this networking solution is made by javidx9 (the one lone coder).
// i have made changes to the code to make it fit my
// means and do not claim to have created all the content
// in the folder this file resides in.