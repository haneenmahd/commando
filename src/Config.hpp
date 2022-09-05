#pragma once

#include <optional>
#include <chrono>
#include <unordered_map>
#include <string>
#include <filesystem>
#include <functional>

struct Config { 
    std::optional<std::string> projectName;
    // a target could be either a file or a directory
    std::optional<std::string> target;
};