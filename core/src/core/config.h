#pragma once

#include "core/log.h"

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>
#include <fstream>
#include <cassert>

bool FileExists(const std::string& file) {
    return std::ifstream(file.c_str()).good();
}

class Config {
public:
    Config(const std::string& file) {
        if (!s_instance) {
            s_instance = this;
        }
        else {
            return;
        }

        YAML::Node node{};
        if (!FileExists(file)) { //TODO create enum to split between config types (sender, reciever, etc)
            STATS_Core_INFO("[Config] no config file found. creating new");

            std::vector<std::string> defaultCons{};
            defaultCons.emplace_back("localhost");

            for (const auto& item : defaultCons) {
                node["connections"].push_back(item);
            }

            node["DefaultPort"] = 5999;

            std::ofstream fout(file);
            fout << node;
        }

        node = YAML::LoadFile(file);
        YAML::Node cons = node["connections"];

        for (YAML::iterator it = cons.begin(); it != cons.end(); it++) {
            connections.emplace_back(it->as<std::string>());
        }

        if (node["DefaultPort"].IsDefined()) {
            DefaultPort = node["DefaultPort"].as<int>();
        } else {
            DefaultPort = 5999;
        }
    }

    ~Config() {
        YAML::Node node{};

        for (const auto& item : connections) {
            node["connections"].push_back(item);
        }

        node["DefaultPort"] = DefaultPort;
    }

    static Config* Get() {
        return s_instance;
    }

    const std::vector<std::string>& Cons() const {
        return connections;
    }

    const int& Port() const {
        return DefaultPort;
    }

private:
    static Config* s_instance;

    std::vector<std::string> connections{};
    int DefaultPort{-1};

};

Config* Config::s_instance = nullptr;
