#pragma once

#include <filesystem>
#include <chrono>
#include <thread>
#include <unordered_map>
#include <string>
#include <functional>

enum WatcherStatus
{
    created,
    modified,
    erased
};

class Watcher
{
public:
    std::string path_to_watch;
    std::chrono::duration<int, std::milli> delay;

    Watcher(std::string path_to_watch, std::chrono::duration<int, std::milli> delay)
    {
        this->path_to_watch = path_to_watch;
        this->delay = delay;
        
        for (auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
        {
            paths_[file.path().string()] = std::filesystem::last_write_time(file);
        }
    }

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
                    it = paths_.erase(it);
                    action(it->first, WatcherStatus::erased);
                }
                else
                {
                    it++;
                }
            }

            // Check if a file was created or modified
            for (auto &file : std::filesystem::recursive_directory_iterator(path_to_watch))
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

private:
    std::unordered_map<std::string, std::filesystem::file_time_type> paths_;
    bool running_ = true;

    bool contains(const std::string &key)
    {
        auto el = paths_.find(key);
        return el != paths_.end();
    }
};