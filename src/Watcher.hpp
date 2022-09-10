#pragma once

#include <iostream>
#include <chrono>
#include <sys/stat.h>
#include "DirectoryWatcher.hpp"
#include "FileWatcher.hpp"
#include "WatcherStatus.hpp"

enum WatcherTargetType
{
    file,
    directory,
    symlink,
    unknown
};

class Watcher
{
public:
    WatcherTargetType targetType;
    FileWatcher fw;
    DirectoryWatcher dw;

    Watcher(std::string path_To_watch, std::chrono::duration<int, std::milli> delay)
    {
        targetType = findTargetType(path_To_watch);

        if (targetType == file)
        {
            fw = FileWatcher{path_To_watch, delay};

            std::cout << fw.file_to_watch << std::endl;
        }
        else if (targetType == directory)
        {
            dw = DirectoryWatcher{path_To_watch, delay};
        }
        else
        {
            std::cout << "Sorry, symlinks or unknown file types are not supported as of now. :>" << std::endl;

            exit(EXIT_FAILURE);
        }
    };

    void start(const std::function<void(std::string, WatcherStatus)> &action)
    {
        if (targetType == file)
        {
            fw.start(action);
        }
        else
        {
            dw.start(action);
        }
    }

    // function to start observing the filesystem
    static void startWatching(Config config)
    {
        Watcher w{config.target, std::chrono::milliseconds(500)};

        w.start([config](std::string filename, WatcherStatus status)
        {
            switch (status)
            {
                case WatcherStatus::created:
                system(config.createdCommand.c_str());
                break;

                case WatcherStatus::modified:
                system(config.modifiedCommand.c_str());
                break;
                case WatcherStatus::erased:
                system(config.erasedCommand.c_str());
                break;
                default:
                std::cout << "Error! Unknown file status.\n";
            }
        });
    }

private:
    WatcherTargetType findTargetType(std::string path_to_watch)
    {
        struct stat status;

        if (lstat(path_to_watch.c_str(), &status) == 0)
        {
            if (S_ISDIR(status.st_mode))
            {
                return directory;
            }
            else if (S_ISREG(status.st_mode))
            {
                // file
                return file;
            }
            else if (S_ISLNK(status.st_mode))
            {
                // symlink
                return symlink;
            } else {
                std::cout << "The target: " << path_to_watch << " has a unknown file kind, which is currently not supported by watcher. If you're wondering why it is not supported, it is because this might result in unexpected bugs in the program." << std::endl;

                exit(EXIT_FAILURE);
                
                return unknown;
            }
        }

        return unknown;
    }
};
