#pragma once

#include "core/base.h"
#include "core/log.h"
#include "core/config.h"
#include "stats/stat.h"

#include "core/Networking/net_common.h"


class StatSender : public net::server_interface<net::MessageType> {
public:
    StatSender(uint16_t port) : net::server_interface<net::MessageType>(port) { StatsCore::Log::Init(); }

protected:
    virtual bool OnClientConnect(std::shared_ptr<net::connection<net::MessageType>> client) {
        net::message<net::MessageType> msg;
        msg.header.id = net::MessageType::ServerAccept;
        client->SendMsg(msg);
        return true;
    }

    // called when a client appears to have disconnected
    virtual void OnClientDisconnect(std::shared_ptr<net::connection<net::MessageType>> client) {
        STATS_Core_INFO("Removing Client [{}]", client->GetID());
    }

    virtual void OnMessage(std::shared_ptr<net::connection<net::MessageType>> client,
                           net::message<net::MessageType>& msg) {
        net::message<net::MessageType> message;

        switch (msg.header.id) {
            case net::MessageType::ServerPing:
                {
                    //std::cout << "[" << client->GetID() << "]: ServerPing\n";
                    STATS_Core_INFO("[{}]: ServerPing", client->GetID());
                    // simply bounce message back to client
                    client->SendMsg(msg);

                    break;
                }
            case net::MessageType::ServerSendStats:
                {
                    int threadCount = -1;
                    float avg1 = -1;
                    float avg5 = -1;
                    float avg15 = -1;
                    float rawCpuTemp = -1;
                    int totalMem = -1;
                    int freeMem = -1;
                    
                    if (!Stats::StatInfo::GetCpuStats(threadCount, avg1, avg5, avg15, rawCpuTemp)){
                        STATS_Core_WARN("Failed to get all CPU stats");
                    }
                    if (!Stats::StatInfo::GetMemoryStats(totalMem, freeMem)){
                        STATS_Core_WARN("Failed to get all Memory stats");
                    }

                    msg << freeMem << totalMem << rawCpuTemp << avg15 << avg5 << avg1 << threadCount;

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
