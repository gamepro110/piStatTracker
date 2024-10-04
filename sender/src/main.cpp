#include "main.h"

#include <iostream>

int main() {
    StatsCore::Log::Init();
    Config conf("config.yaml");
    STATS_Core_INFO("[config] port: {}", conf.Port());

    StatSender sender = StatSender(conf.Port());

    if (sender.Start()) {
        while (1) {
            sender.Update(2,true);
        }
    }
    else {
        STATS_Core_INFO("failed to start server");
    }
    
    std::cin.get();
    return 0;
}
