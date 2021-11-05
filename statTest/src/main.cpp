#include "main.h"

int main()
{
    Stats stats;
    float temp = stats.GetCpuTemp();
    auto freqs = stats.GetFreqs();
    float minfreq = freqs.min;
    float maxfreq = freqs.max;
    float curfreq = freqs.cur;
    RamStats mem = stats.GetRamInfo();
    int totalMem = mem.total;
    int freeMem = mem.free;
    int availableMem = mem.available;
    float val = ((totalMem - freeMem) / (float)totalMem);

    // TODO use line below to get disk data
    // "df -h | grep '/dev/root'"
    // TODO add get ram info to stats

    std::cout << "tip: -1 == error.\n";
    std::cout << "temp: " << temp << "\n";
    std::cout << "freqs (min/max/cur): (" << minfreq << "/" << maxfreq << "/" << curfreq << ")\n";
    std::cout << "num cpu cores: " << stats.GetProcessorCount() << "\n";
    std::cout << "mem (total/free/available): (" << totalMem << "/" << freeMem << "/" << availableMem << ") MBs\n";
    std::cout << "mem use%: " << std::roundf(val * 100) / 100 << "%\n";

    return 0;
}
