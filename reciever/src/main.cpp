#include "main.h"

int main() {
    bool run = true;
    std::vector<ImGUIWindow::fontWraper> fonts;
    ImGUIWindow::ImGuiWindow window{ "Stat monitor" };
    std::vector<ConnectionPair> connections;

    { //TODO replace with yaml file
        connections.emplace_back("localhost", 5999);
    }

    for (auto& conn : connections) {
        if (!conn.reciever->Connect(conn.address, conn.port)) {
            Logs::LogMsg(std::string("failed to connect -> " + conn.address), LogLevel::Warning);
            continue;
        }
    }

    if (!connections.size()) {
        return -1;
    }

    ImGUIWindow::CallbackArg<bool*, std::vector<ConnectionPair>*> mainLogicCallback{
        &mainLogic,
        &run,
        &connections
    };
    ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*> VisualizationLogicCallback{
        &VisualizationLogic,
        &connections
    };
    ImGUIWindow::CallbackVoid LogWindowCallback {
        &LogWindow
    };

    ImGUIWindow::CallbackArg<> onTimedEventCallback{
        &onTimedEvent
    };

    ImGUIWindow::CallbackArg<ImGUIWindow::CallbackArg<>*> TimedEventsCallback{
        &TimedEvents,
        &onTimedEventCallback
    };

    
    window.AddSubWindow(&mainLogicCallback);
    window.AddSubWindow(&VisualizationLogicCallback);
    window.AddSubWindow(&LogWindowCallback);
    window.AddSubWindow(&TimedEventsCallback);

    if (!window.Start(fonts)) {
        std::cout << "Failed to start Window!\n";
        return -1;
    }
    window.Update();

    #if false
    while (true)
    {
        for (auto& item : recievers)
        {
            if (item->IsConnected())
            {
                if (!item->Incomming().empty())
                {
                    net::message<net::MessageType> msg = item->Incomming().pop_front().msg;

                    switch (msg.header.id)
                    {
                    case net::MessageType::ServerAccept:
                        std::cout << "[Sender] Connection Accepted\n";
                        break;

                    case net::MessageType::ServerDeny:
                    {
                        std::cout << "[Sender] Connection Denied\n";
                        //recievers.erase(std::remove(recievers.begin(), recievers.end(), item), recievers.end());
                        break;
                    }
                    case net::MessageType::ServerPing:
                        std::cout << "[Sender] Ping\n";
                        break;
                    
                    default:
                        break;
                    }
                }
            }
        }
    }
    #endif

    return 0;
}