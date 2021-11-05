#pragma once
#include <ncurses.h>
#include <fstream>
#include <math.h>
#include <regex>
#include <string>
#include <thread>
#include <iostream>

struct MinMaxFreq
{
    MinMaxFreq(float _min, float _cur, float _max) :
        min(_min), cur(_cur), max(_max)
    {
    }

    float min;
    float cur;
    float max;
};

struct RamStats
{
    RamStats(int _total, int _free, int _available) : 
        total(_total), free(_free), available(_available)
    {
    }

    int total;
    int free;
    int available;
};

struct StorageStats
{
    StorageStats(float _totalSize, float _used, float _available, float _usedPercentage, const std::string& _mountingPoint) : 
        totalSize(_totalSize), used(_used), available(_available), usedPercentage(_usedPercentage), mountingPoint(_mountingPoint)
    {
    }

    float totalSize;
    float used;
    float available;
    float usedPercentage;
    std::string mountingPoint;
};

struct FileReader
{
    static std::string ReadLine(const std::string& fileName)
    {
        std::string output = "";

        std::fstream file;
        file.open(fileName.c_str(), std::ios::in);
        if (file.is_open())
        {
            std::string text;
            while (std::getline(file, text))
            {
                output += text;
            }
            file.close();
        }

        return output;
    }
};

struct Stats
{
    const StorageStats GetStorageStats()
    {
        const std::string regexPatern{"^/dev/[a-z]{1,}"};
        const std::string cmd{"df -h | grep '/dev/' > /home/pi/storageStats.txt"};

        float totalSize;
        float used;
        float available;
        float usedPercentage;
        std::string mountingPoint;

        system(cmd.c_str());

        std::fstream file;
        file.open("/home/pi/storageStats.txt", std::ios::in);
        if (file.is_open())
        {
            std::string txt{};
            while (std::getline(file, txt))
            {
                //TODO continue reading stats out of file
            }

            file.close();
        }

        system("rm /home/pi/storageStats.txt");

        return StorageStats(totalSize, used, available, usedPercentage, mountingPoint);
    }
    
    const RamStats GetRamInfo()
    {
        const std::string regexPatern{"([0-9]{3,8})"};
        const std::string fileName{"/proc/meminfo"};
        int total = -1;
        int free = -1;
        int available = -1;

        std::fstream file;
        std::string text;
        file.open(fileName.c_str(), std::ios::in);
        if (file.is_open())
        {
            std::getline(file, text);
            total = static_cast<int>(std::stof(GetStringAfterRegex(text, regexPatern)));

            std::getline(file, text);
            free = static_cast<int>(std::stof(GetStringAfterRegex(text, regexPatern)));

            std::getline(file, text);
            available = static_cast<int>(std::stof(GetStringAfterRegex(text, regexPatern)));

            file.close();
        }

        return RamStats(total, free, available);
    }

    const std::string GetStringAfterRegex(const std::string& str, const std::string& regexExpression)
    {
        std::regex r{regexExpression};
        std::smatch match;

        if (std::regex_search(str, match, r))
        {
            for (const auto& item : match)
            {
                return item.str();
            }
        }

        return "";
    }

    const float GetCpuTemp()
    {
        float temp = -1;
        const std::string tempratureFile = "/sys/class/thermal/thermal_zone0/temp";
        std::fstream file;
        file.open(tempratureFile.c_str(), std::ios::in);
        if (file.is_open())
        {
            std::string text;
            bool found = false;
            while (std::getline(file, text) && !found)
            {
                temp = std::stof(text);
                //cout << tp << "\n";
                if (temp != -1)
                {
                    found = true;
                    temp /= 1000;
                }
            }

            file.close();
        }

        return temp;
    }

    const MinMaxFreq GetFreqs() // cleanup required
    {
        float min = -1;
        float cur = -1;
        float max = -1;
        std::string regexExpr{"([0-9]{3,})"};

        #pragma region min_freq
        system("lscpu | grep 'min' > /home/pi/freqmin.txt");
        std::fstream file1;
        file1.open("/home/pi/freqmin.txt", std::ios::in);

        if (file1.is_open())
        {
            std::string text;
            bool found = false;
            while (std::getline(file1, text) && !found)
            {
                std::string value = GetStringAfterRegex(text, regexExpr);
                min = std::stof(value);
                found = min != -1;
            }

            file1.close();
        }
        system("rm /home/pi/freqmin.txt");
        #pragma endregion

        #pragma region max_freq
        system("lscpu | grep 'max' > /home/pi/freqmax.txt"); // reading too fast?
        std::fstream file2;
        file2.open("/home/pi/freqmax.txt", std::ios::in);
        if (file2.is_open())
        {
            std::string text;
            bool found = false;
            while (std::getline(file2, text) && !found)
            {
                std::string value = GetStringAfterRegex(text, regexExpr);
                max = std::stof(value);
                found = max != -1;
            }
        
            file2.close();
        }
        system("rm /home/pi/freqmax.txt");
        #pragma endregion

        #pragma region cur_freq
        std::fstream file3;
        file3.open("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", std::ios::in);
        if (file3.is_open())
        {
            std::string text;
            bool found = false;
            while (std::getline(file3, text) && !found)
            {
                cur = std::stof(text);
                if (cur != -1)
                {
                    cur /= 1000;
                    found = true;
                }
            }

            file3.close();
        }

        #pragma endregion

        return MinMaxFreq(min, cur, max);
    }

    const int GetProcessorCount()
    {
        return std::thread::hardware_concurrency();
    }
};