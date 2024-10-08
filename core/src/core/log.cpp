#include "log.h"

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

std::shared_ptr<spdlog::logger> StatsCore::Log::s_clientLogger = nullptr;
std::shared_ptr<spdlog::logger> StatsCore::Log::s_coreLogger = nullptr;

void StatsCore::Log::Init() {
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("StatTracker.log", true));

    logSinks[0]->set_pattern("%^[%T] %n: %v%$");
    logSinks[1]->set_pattern("[%T] [%l] %n: %v");

    s_coreLogger = std::make_shared<spdlog::logger>("Core", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_coreLogger);
    s_coreLogger->set_level(spdlog::level::trace);
    s_coreLogger->flush_on(spdlog::level::trace);

    s_clientLogger = std::make_shared<spdlog::logger>("App", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_clientLogger);
    s_clientLogger->set_level(spdlog::level::trace);
    s_clientLogger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& StatsCore::Log::GetCoreLogger() {
    return s_coreLogger;
}

std::shared_ptr<spdlog::logger>& StatsCore::Log::GetClientLogger() {
    return s_clientLogger;
}
