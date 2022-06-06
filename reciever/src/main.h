#pragma once

#include "core/base.h"
#include "core/config.h"

#include "ImGuiWindow.h"
#include "Callbacks.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

#include <string>
#include <string_view>
#include <algorithm>

#include <chrono>

class StatReciever : public net::client_interface<net::MessageType> {
public:
    ~StatReciever() { }

    auto operator<=>(const StatReciever& rhs) const = default;

public:
    void ServerPing() {
        net::message<net::MessageType> msg;
        msg.header.id = net::MessageType::ServerPing;

        std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();

        msg << timeNow;
        SendMsg(msg);
    }

    void GetStats() {
        STATS_Core_INFO("Called GetStats()");
    }
};

struct ConnectionPair {
    ConnectionPair(const std::string& ip, int port) :
        address(ip),
        port(port) {
        reciever = new StatReciever();
    }

    ~ConnectionPair() {
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
        bool curConnected = false;
        for (const auto& item : (*con)) {
            curConnected = item.reciever->IsConnected();
            ImGui::Text(std::string("\"" + item.address + "\" (con: " + (curConnected ? "Y" : "N") + ")").c_str());
        }
    }
    ImGui::End();
}

template<typename ... Args> void MainMenuBarLogic(Args... args) {
    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);
    std::vector<ConnectionPair>* con = wrapper.template Get<std::vector<ConnectionPair>*, 0>();

    /*
    static bool File_Exit = false;
    static bool Connections_Add = false;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Connections")) {
            ImGui::MenuItem("Add (WIP)", NULL, &Connections_Add);

            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    {  // Connections
        if (Connections_Add) {
            Connections_Add = false;
            ImGui::OpenPopup("Connection_Add");
        }


        if (ImGui::BeginPopup("Connection_Add")) {
            static char newConnTxt[30] = "";
            auto ClosePopup = []() {
                ImGui::CloseCurrentPopup();
            };
            
            ImGui::TextUnformatted("Add Connection");
            ImGui::InputText("ip:", newConnTxt, 30);
            
            ImGui::BeginGroup();
            if (ImGui::Button("Add")) {
                std::string txt = "[Sender] Adding \'";
                txt += newConnTxt;
                txt += "\' connection";
                STATS_INFO(txt);
                auto item = ConnectionPair(newConnTxt, Config::Get()->Port());
                if (item.reciever->Connect(item.address, item.port)) { // TODO fix connecting issue
                    std::string txt = "[Sender] \'";
                    txt += newConnTxt;
                    txt += "\' connected successfully";

                    STATS_INFO(txt);
                    con->push_back(item);
                }
                else {
                    std::string txt = "[Sender] failed to connect -> \'";
                    txt += newConnTxt;
                    txt += "\'";

                    STATS_WARN(txt);
                }
                std::strcpy(newConnTxt, "");
                ClosePopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                ClosePopup();
            }
            ImGui::EndGroup();
            ImGui::EndPopup();
        }
    }
    //*/
}

template<typename ... Args>
void mainLogic(Args ... args) {
    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);

    bool* run = wrapper.template Get<bool*, 0>();
    std::vector<ConnectionPair>* con = wrapper.template Get<std::vector<ConnectionPair>*, 1>();

    for (const auto& item : (*con)) {
        if (item.reciever->IsConnected()) {
            if (!item.reciever->Incomming().empty()) {
                net::message<net::MessageType> msg = item.reciever->Incomming().pop_front().msg;

                switch (msg.header.id)
                {
                case net::MessageType::ServerAccept:
                    STATS_INFO("[Sender] Connection Accepted");
                    break;

                case net::MessageType::ServerDeny:
                    STATS_INFO("[Sender] Connection Denied");
                    break;

                case net::MessageType::ServerPing:
                    STATS_INFO("[Sender] Ping");
                    break;

                case net::MessageType::ServerSendStats:
                    STATS_INFO("[Sender] MessageType::ServerSendStats:");
                    break;
                
                default:
                    break;
                }
            }
        }
    }
}

template<typename... Args>
void onTimedEvent(Args... args) {
    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);
    std::vector<ConnectionPair>* con = wrapper.template Get<std::vector<ConnectionPair>*, 0>();
    
    for (const auto& item : (*con)) {
        item.reciever->GetStats();
    }
}

template<typename ... Args>
void TimedEvents(Args... args) {
    using namespace std::chrono_literals;

    ImGUIWindow::ArgUnwrapper<Args...> wrapper(args...);
    ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*>* cb = wrapper.template Get<ImGUIWindow::CallbackArg<std::vector<ConnectionPair>*>*, 0>();

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
    else if (!resetTimer && !canReset) {
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
