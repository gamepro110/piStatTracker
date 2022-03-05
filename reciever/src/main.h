#pragma once
#include "Networking/net.h"

#include <iostream>
#include <algorithm>

class StatReciever : public net::client_interface<net::MessageType>
{
public:
	~StatReciever()
	{}

	auto operator<=>(const StatReciever& rhs) const = default;

public:
	void ServerPing()
	{
		net::message<net::MessageType> msg;
		msg.header.id = net::MessageType::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		SendMsg(msg);
	}
};
