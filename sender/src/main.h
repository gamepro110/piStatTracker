#pragma once

#include "Networking/net.h"

#include <iostream>
#include <thread>

class StatSender : public net::server_interface<net::MessageType>
{
  public:
    StatSender(uint16_t port) : net::server_interface<net::MessageType>(port)
    {
    }

  protected:
    virtual bool OnClientConnect(std::shared_ptr<net::connection<net::MessageType>> client)
    {
        net::message<net::MessageType> msg;
        msg.header.id = net::MessageType::ServerAccept;
        client->SendMsg(msg);
        return true;
    }

    // called when a client appears to have disconnected
    virtual void OnClientDisconnect(std::shared_ptr<net::connection<net::MessageType>> client)
    {
        std::cout << "Removing Client [" << client->GetID() << "]\n";
    }

    virtual void OnMessage(std::shared_ptr<net::connection<net::MessageType>> client,
                           net::message<net::MessageType>& msg)
    {
        net::message<net::MessageType> message;

        switch (msg.header.id)
        {
            case net::MessageType::ServerPing:
                {
                    std::cout << "[" << client->GetID() << "]: ServerPing\n";
                    // simply bounce message back to client
                    client->SendMsg(msg);

                    break;
                }
            case net::MessageType::ServerSendStats:
                {
                    message << std::thread::hardware_concurrency();

                    client->SendMsg(message);
                    break;
                }
            default:
                {
                    break;
                }
        }
    }
};
