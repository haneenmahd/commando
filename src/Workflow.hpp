#pragma once

#include <filesystem>
#include <chrono>
#include <unordered_map>
#include <string>
#include <functional>

enum WorkflowError {
    file_not_found,
    invalid,
    valid
};

class Workflow
{
    public:
        std::string path_to_watch;
        std::string config;

        std::string read_config_file() {}
};