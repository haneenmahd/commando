#pragma once

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>
#include <sys/stat.h>
#include "WatcherStatus.hpp"

class DirectoryWatcher
{
public:
    std::string dir_to_watch;
    std::chrono::duration<int, std::milli> delay;

    DirectoryWatcher(std::string dir_to_watch, std::chrono::duration<int, std::milli> delay)
    {
        this->dir_to_watch = dir_to_watch;
        this->delay = delay;

        for (auto &file : std::filesystem::recursive_directory_iterator(dir_to_watch))
        {
            paths_[file.path().string()] = std::filesystem::last_write_time(file);
        }
    }

    DirectoryWatcher() {}; // just to silence the error for default constructor

    void start(const std::function<void(std::string, WatcherStatus)> &action)
    {
        while (running_)
        {
            // Wait for delay
            std::this_thread::sleep_for(delay);

            auto it = paths_.begin();
            while (it != paths_.end())
            {
                if (!std::filesystem::exists(it->first))
                {
                    action(it->first, WatcherStatus::erased);
                    it = paths_.erase(it);
                }
                else
                {
                    it++;
                }
            }

            // Check if a file was created or modified
            for (auto &file : std::filesystem::recursive_directory_iterator(dir_to_watch))
            {
                auto current_file_last_write_time = std::filesystem::last_write_time(file);

                // File creation
                if (!contains(file.path().string()))
                {
                    paths_[file.path().string()] = current_file_last_write_time;
                    action(file.path().string(), WatcherStatus::created);
                }
                // File modified
                else
                {
                    if (paths_[file.path().string()] != current_file_last_write_time)
                    {
                        paths_[file.path().string()] = current_file_last_write_time;
                        action(file.path().string(), WatcherStatus::modified);
                    }
                }
            }
        }
    }

    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    bool running_ = true;

    bool contains(const std::string &key)
    {
        auto el = paths_.find(key);
        return el != paths_.end();
    }
};