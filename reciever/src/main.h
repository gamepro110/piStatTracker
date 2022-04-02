#pragma once
#include "Networking/net.h"
#include "ImGuiWindow.h"
#include "Callbacks.h"

#include <iostream>
#include <string>
#include <string_view>
#include <algorithm>

#include <chrono>

enum class LogLevel
{
    Info,
    Warning,
    Error
};
struct Log
{
    Log(const std::string& msg, LogLevel lvl) :
        msg(msg),
        lvl(lvl)
    {}

    std::string msg;
    LogLevel lvl;
};
class Logs
{
public:
    static void LogMsg(const std::string& str, LogLevel lvl = LogLevel::Info) {
        logs.emplace_back(Log(str, lvl));
    }

    static std::vector<Log> GetLogs() {
        return logs;
    }

private:
    static std::vector<Log> logs;
};

std::vector<Log> Logs::logs = {};

class StatReciever : public net::client_interface<net::MessageType>
{
public:
	~StatReciever()
	{}

	auto operator<=>(const StatReciever& rhs) const = default;

public:
	void ServerPing()
	{
		net::message<net::MessageType> msg;
		msg.header.id = net::MessageType::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

		msg << timeNow;
		SendMsg(msg);
	}
};

struct ConnectionPair
{
    ConnectionPair(const std::string& ip, int port) :
        address(ip),
        port(port)
    {
        reciever = new StatReciever();
    }
    ~ConnectionPair()
    {
        delete reciever;
    }

    std::string address;
    int port;
    StatReciever* reciever = nullptr;
};

template<typename ... Args>
void VisualizationLogic(Args... args) {
    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);

    std::vector<ConnectionPair>* con = wrapper.template Get<std::vector<ConnectionPair>*, 0> ();

    ImGui::Begin("connected devices", NULL);
    {
        for (const auto& item : (*con)) {
            ImGui::Text(std::string("\"" + item.address + "\" (con: " + 
                (item.reciever->IsConnected() ? "Y" : "N") + ")").c_str());
        }
    }
    ImGui::End();
}

void LogWindow() {
    std::vector<Log> logs = Logs::GetLogs();
    static const auto colCyan = ImVec4(0.0f, 1.0f, 1.0f, 1.0f);
    static const auto colYellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    static const auto colRed = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

    ImGui::Begin("Logs", NULL);
    {
        ImGui::Text(std::string("log size: " + std::to_string(logs.size()) + " ##").c_str());
        
        if (ImGui::BeginChild("")) {
            for (const auto& log : logs) {
                static ImVec4 col;
                switch (log.lvl)
                {
                    case LogLevel::Info:
                        col = colCyan;
                        break;
                    case LogLevel::Warning:
                        col = colYellow;
                        break;
                    case LogLevel::Error:
                        col = colRed;
                        break;
                }
                ImGui::TextColored(col, log.msg.c_str());
            }
            ImGui::EndChild();
        }

    }
    ImGui::End();
}

template<typename ... Args>
void mainLogic(Args ... args)
{
    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);

    bool* run = wrapper.template Get<bool*, 0>();
    std::vector<ConnectionPair>* con = wrapper.template Get<std::vector<ConnectionPair>*, 1>();

    for (const auto& item : (*con))
    {
        if (item.reciever->IsConnected())
        {
            if (!item.reciever->Incomming().empty())
            {
                net::message<net::MessageType> msg = item.reciever->Incomming().pop_front().msg;

                switch (msg.header.id)
                {
                case net::MessageType::ServerAccept:
                    Logs::LogMsg("[Sender] Connection Accepted", LogLevel::Info);
                    break;

                case net::MessageType::ServerDeny:
                    Logs::LogMsg("[Sender] Connection Denied", LogLevel::Warning);
                    break;

                case net::MessageType::ServerPing:
                    Logs::LogMsg("[Sender] Ping", LogLevel::Info);
                    break;
                
                default:
                    break;
                }
            }
        }
    }
}

template<typename... Args>
void onTimedEvent(Args... args)
{
    Logs::LogMsg("on event...");
}


template<typename ... Args>
void TimedEvents(Args... args) {
    using namespace std::chrono_literals;

    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);
    ImGUIWindow::CallbackArg<>* cb = wrapper.template Get<ImGUIWindow::CallbackArg<>*, 0>();

    static bool canReset = true;
    static bool resetTimer = false;
    static std::chrono::steady_clock::time_point begin_time = std::chrono::steady_clock::now();
    static std::chrono::steady_clock::time_point end_time;
    static std::chrono::steady_clock::duration elapsed;
    static auto eTime = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    
    end_time = std::chrono::steady_clock::now();
    elapsed = end_time - begin_time;
    eTime = std::chrono::duration_cast<std::chrono::seconds>(elapsed);

    if (eTime > 5s) {
        resetTimer = true;
        //Logs::LogMsg("5 sec pased, reset timer");
        static_cast<ImGUIWindow::CallbackBase*>(cb)->Invoke();
    }

    if (resetTimer && canReset) {
        canReset = false;
        resetTimer = false;
        begin_time = std::chrono::steady_clock::now();
    }
    else if (!resetTimer && !canReset)
    {
        canReset = true;
    }
}

template<typename T>
int GetIndex(const std::vector<T>& vec, const T& b) {
    auto it = std::find(vec.begin(), vec.end(), b);
    if (it != vec.end()) {
        return (it - vec.end());
    }
    else {
        return -1;
    }
}
