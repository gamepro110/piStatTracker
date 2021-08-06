#include "main.h"

#include <iostream>

int main() {
    StatSender sender = StatSender(5999);

    if (sender.Start()) {
        while (1) {
            sender.Update(-1,true);
        }
    }
    else {
        std::cout << "failed to start server\n";
    }
    
    std::cin.get();
    return 0;
}