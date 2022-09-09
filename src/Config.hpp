#pragma once

#include <optional>
#include <chrono>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <functional>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Config
{
    std::string projectName;
    // a target could be either a file or a directory
    std::string target;
    // Commands
    std::string createdCommand;
    std::string modifiedCommand;
    std::string erasedCommand;
};

class ConfigReader
{
public:
    static Config read(std::string config_path)
    {
        struct Config config;
        std::ifstream rawConfig(config_path);

        json configContent = json::parse(rawConfig);

        config.projectName = configContent["projectName"];
        config.target = configContent["target"];
        config.createdCommand = configContent["createdCommand"];
        config.modifiedCommand = configContent["modifiedCommand"];
        config.erasedCommand = configContent["erasedCommand"];

        return config;
    }
};