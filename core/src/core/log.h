#pragma once

#pragma warning(push, 0)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

#include <memory>

namespace StatsCore {
    class Log {
    public:
        static void Init();

    public:
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();
        static std::shared_ptr<spdlog::logger>& GetClientLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_coreLogger;
        static std::shared_ptr<spdlog::logger> s_clientLogger;
    };
}

// Core log macros
#define STATS_Core_TRACE(...)		::StatsCore::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define STATS_Core_INFO(...)		::StatsCore::Log::GetCoreLogger()->info(__VA_ARGS__)
#define STATS_Core_WARN(...)		::StatsCore::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define STATS_Core_ERROR(...)		::StatsCore::Log::GetCoreLogger()->error(__VA_ARGS__)
#define STATS_Core_CRITICAL(...)	::StatsCore::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define STATS_TRACE(...)		    ::StatsCore::Log::GetClientLogger()->trace(__VA_ARGS__)
#define STATS_INFO(...)			    ::StatsCore::Log::GetClientLogger()->info(__VA_ARGS__)
#define STATS_WARN(...)			    ::StatsCore::Log::GetClientLogger()->warn(__VA_ARGS__)
#define STATS_ERROR(...)		    ::StatsCore::Log::GetClientLogger()->error(__VA_ARGS__)
#define STATS_CRITICAL(...)		    ::StatsCore::Log::GetClientLogger()->critical(__VA_ARGS__)
