#pragma once
#include "Networking/net.h"
#include <iostream>

class StatReciever : public net::client_interface<net::MessageType>
{
public:
	~StatReciever()
	{}

    bool operator==(const StatReciever& other) {
        if (this->thrContext.get_id() == other.thrContext.get_id()) {
            return true;
        }
        return false;
    }

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
