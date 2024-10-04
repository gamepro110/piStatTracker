#include "main.h"

int main() {
    StatsCore::Log::Init();
    Config conf("config.yaml");

    bool run = true;
    std::vector<ImGUIWindow::fontWraper> fonts;
    ImGUIWindow::ImGuiWindow window{ "Stat monitor" };
    std::vector<ConnectionPair> connections;
    int port = conf.Port();
    for (const auto& item : conf.Cons()) {
        connections.emplace_back(ConnectionPair(item, port));
    }

    for (auto& conn : connections) {
        if (!conn.reciever->Connect(conn.address, conn.port)) {
            STATS_WARN("failed to connect -> ", conn.address);
            continue;
        }
    }

    if (!connections.size()) {
        return -1;
    }

    ImGUIWindow::CallbackArg<bool*, std::vector<ConnectionPair>*> mainLogicCallback {
        &mainLogic,
        &run,
        &connections
    };
    ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*> VisualizationLogicCallback {
        &VisualizationLogic,
        &connections
    };
    ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*> onTimedEventCallback {
        &onTimedEvent,
        &connections
    };
    ImGUIWindow::CallbackArg<ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*>*> TimedEventsCallback {
        &TimedEvents,
        &onTimedEventCallback
    };
    ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*> mainMenuBarCallback{
        &MainMenuBarLogic,
        &connections
    };
    
    window.AddSubWindow(&mainLogicCallback);
    window.AddSubWindow(&VisualizationLogicCallback);
    window.AddSubWindow(&TimedEventsCallback);
    window.AddSubWindow(&mainMenuBarCallback);

    if (!window.Start(fonts)) {
        std::cout << "Failed to start Window!\n";
        return -1;
    }
    window.Update();

    return 0;
}
