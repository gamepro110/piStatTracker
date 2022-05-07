#include "stats/stat.h"

#include <fstream>
#include <math.h>
#include <regex>
#include <string>
#include <thread>
#include <iostream>

//TODO remove selfmade Asio cmake config

int main() {
    int tc = -1;
    float avg1 = -1;
    float avg5 = -1;
    float avg15 = -1;
    float ct = -1;

    // TODO add get ram info to stats
    if (!Stats::StatInfo::GetCpuStats(tc, avg1, avg5, avg15, ct)) {
        std::cout << "failed to read cpu stats\n";
    }
    else {
        std::cout << "tip: -1 == error.\n\n";
        std::cout << "threadCount: " << tc << "\n";
        std::cout << "average 1 min: " << avg1 << "\n";
        std::cout << "average 5 min: " << avg5 << "\n";
        std::cout << "average 15 min: " << avg15 << "\n";
        std::cout << "cpu temp C0: " << ct << " c" << "\n";
    }

    int memTotal = -1;
    int memAvailable = -1;

    if (!Stats::StatInfo::GetMemoryStats(memTotal, memAvailable)) {
        std::cout << "failed to read ram stats\n";
    }
    else {
        std::cout << "total ram: " << memTotal << "\n";
        std::cout << "used ram: " << memAvailable << "\n";
        printf("free ram: %.3f%%\n", (( float )memAvailable / memTotal) * 100);
    }

    return 0;
}
