#include "main.h"

#include <iostream>

int main() {
    Config conf("config.yaml");
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
