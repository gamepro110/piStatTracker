#pragma once

#define YAML_CPP_STATIC_DEFINE
#include "yaml-cpp/yaml.h"

#include <vector>
#include <string>
#include <fstream>

bool FileExists(const std::string& file) {
	return std::ifstream(file.c_str()).good();
}

class Config {
public:
    Config(const std::string& file) {
	    if (!FileExists(file)) {
		YAML::Node node {};
		std::vector<std::string> defaultCons {};
		defaultCons.emplace_back("localhost");
		
		for (const auto& item : defaultCons) {
		    node["connections"].push_back(item);
		}

		std::ofstream fout(file);
		fout << node;
	    }
	    
	    YAML::Node node = YAML::LoadFile(file);
	    YAML::Node cons = node["connections"];
	    
	    for (YAML::iterator it = cons.begin(); it != cons.end(); it++) {
		    connections.emplace_back(it->as<std::string>());
	    }
    }

private:
    std::vector<std::string> connections{};

};

