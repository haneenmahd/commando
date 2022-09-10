#pragma once

#include <optional>
#include <chrono>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <functional>
#include <fstream>
#include <nlohmann/json.hpp>
#include "ansi-text/ansi-text.h"

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

        if (!ConfigReader::hasAnyCommand(configContent))
        {
            std::cout << RED_BG << "ERROR" << NORMAL << " You don't have any command specified in the config file." << std::endl;
            std::cout << std::endl;
            std::cout << "Add a '[created|modified|erased]Command' property with a value of the command that should be run inside the config file." << std::endl;
            std::cout << std::endl;
            std::cout << BOLD << "Config File Location: " << NORMAL << GREEN << config_path << NORMAL << std::endl;

            exit(EXIT_FAILURE);
        }
        

        config.projectName = ConfigReader::valueOr(configContent, "projectName", "");
        config.target = ConfigReader::valueOr(configContent, "target", "");
        config.createdCommand = ConfigReader::valueOr(configContent, "createdCommand", "");
        config.modifiedCommand = ConfigReader::valueOr(configContent, "modifiedCommand", "");
        config.erasedCommand = ConfigReader::valueOr(configContent, "erasedCommand", "");

        return config;
    }

    // reads the value from the object, if it doesn't exist return a default value
    static std::string valueOr(json config, std::string propertyName, std::string defaultValue = "") {
        if (!config.contains(propertyName))
        {
            return defaultValue;
        }
        
        return config[propertyName];
    }

    // VERIFICATIONS
    // These functions will help reduce the production of unknown bugs inside the code

    static bool hasAnyCommand(json config) {
        if (!config.contains("createdCommand") 
            && !config.contains("modifiedCommand") 
            && !config.contains("erasedCommand"))
        {
           return false;
        }
        
        return true;
    }
};