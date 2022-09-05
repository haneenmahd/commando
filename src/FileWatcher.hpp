#pragma once

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include "WatcherStatus.hpp"

class FileWatcher
{
public:
    std::string file_to_watch;
    std::chrono::duration<int, std::milli> delay;

    FileWatcher(std::string file_to_watch, std::chrono::duration<int, std::milli> delay)
    {
        this->file_to_watch = file_to_watch;
        this->delay = delay;
    }

    FileWatcher() {}; // just to silence the error for default constructor

    void start(const std::function<void(std::string, WatcherStatus)> &action)
    {
        last_written_time = std::filesystem::last_write_time(file_to_watch);

        while (running_)
        {
            // Wait for delay
            std::this_thread::sleep_for(delay);

            if (!std::filesystem::exists(file_to_watch))
            {
                action(file_to_watch, WatcherStatus::erased);
            }

            // Check if a file was created or modified
            auto current_file_last_write_time = std::filesystem::last_write_time(file_to_watch);

            // File modified
            if (current_file_last_write_time != last_written_time)
            {
                action(file_to_watch, WatcherStatus::modified);
            }

            last_written_time = current_file_last_write_time;
        }
    }

private:
    std::filesystem::file_time_type last_written_time;
    bool running_ = true;
};