#include "stat.h"

namespace Stats {
    bool StatInfo::GetCpuStats(int& threadCount, float& avg1min, float& avg5min, float& avg15min, float& rawCpuTemp) {
        threadCount = std::thread::hardware_concurrency();

        #if defined(STATS_LINUX)

        {
            std::ifstream cpuLoadFile;
            cpuLoadFile.open("/proc/loadavg", std::ios::in);
            if (cpuLoadFile.fail()) {
                printf("failed to open stats\n");
            }
            else {
                cpuLoadFile >> avg1min;
                cpuLoadFile >> avg5min;
                cpuLoadFile >> avg15min;
            }
            cpuLoadFile.close();

            std::ifstream cpuTempFile;
            cpuTempFile.open("/sys/class/thermal/thermal_zone0/temp", std::ios::in);
            if (cpuTempFile.fail()) {
                printf("failed to read temps\n");
            }
            else {
                cpuTempFile >> rawCpuTemp;
                rawCpuTemp /= 1000;

            }
            cpuTempFile.close();
        }

        return true;
        //#elif defined(STATS_WINDOWS)
        #else
        return false;
        #endif
    }
    
    bool StatInfo::GetMemoryStats(int& totalMem, int& freeMem) {

        #if defined(STATS_LINUX)
        {
            FILE* ramread = fopen("/proc/meminfo", "r");
            if (!ramread) {
                printf("failed to open ram stats\n");
                return false;
            }
            fscanf(ramread, "MemTotal:       %i kB\nMemFree:        %i kB", &totalMem, &freeMem);
            fclose(ramread);
        }
        return true;
        #else
        return false;
        #endif
    }

    bool StatInfo::FileExists(const std::string& file) {
        auto ifs = std::ifstream(file.c_str());
        bool isOpened = ifs.good();
        ifs.close();
        return isOpened;
    }
}
