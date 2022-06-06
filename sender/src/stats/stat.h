#pragma once

#include <string>
#include <fstream>
#include <optional>
#include <thread>
#include <regex>

#if defined(STATS_DEBUG)
#elif defined(STATS_DEBUG)
#elif defined(STATS_DEBUG)
#endif

#if defined(STATS_WINDOWS)
#elif defined(STATS_LINUX)
#endif

namespace Stats {
    class StatInfo {
    public:
        static bool GetCpuStats(int& threadCount, float& avg1min, float& avg5min, float& avg15min, float& rawCpuTemp);
        static bool GetMemoryStats(int& totalMem, int& freeMem);

    private:
        static bool FileExists(const std::string& file);

    };
}  // namespace Stats
