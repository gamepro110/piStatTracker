#include "main.h"

int main() {
    std::vector<StatReciever> recievers;

    for (auto& item : recievers)
    {
        if(!item.Connect("", 5999)) // connect to all recievers
        {
            for (auto& it : recievers) // disconnet from all connected recievers if one failed
            {
                it.Disconnect();
                if (it == item)
                {
                    break;
                }
            }
            return 1;
        }
    }

    while (true)
    {
        for (auto& item : recievers)
        {
            if (item.IsConnected())
            {
                if (!item.Incomming().empty())
                {
                    net::message<net::MessageType> msg = item.Incomming().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case net::MessageType::ServerAccept:
                        std::cout << "[Sender] Connection Accepted";
                        break;

                    case net::MessageType::ServerDeny:
                    {
                        std::cout << "[Sender] Connection Denied";
                        std::vector<StatReciever>::iterator it;
                        for (it = recievers.begin(); it != recievers.end(); it++) {
                            if (*it == item) {
                                recievers.erase(it);
                                break;
                            }
                        } // TODO recheck logic

                        break;
                    }
                    case net::MessageType::ServerPing:
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    }
    
    return 0;
}