#include <iostream>
#include "Watcher.hpp"

int main()
{
    Watcher w{"./", std::chrono::milliseconds(500)};

    w.start([](std::string path_to_watch, WatcherStatus status)
    {
        if (!std::filesystem::is_regular_file(path_to_watch)) {
            return;
        } else {
            switch (status)
            {
            case WatcherStatus::created:
                std::cout << "File created: " << path_to_watch << '\n';
                break;

            case WatcherStatus::modified:
                std::cout << "File modified: " << path_to_watch << '\n';
                break;
            case WatcherStatus::erased: 
                std::cout << "File erased: " << path_to_watch << '\n';
                break;
            default:
                std::cout << "Error! Unknown file status.\n";
            }
        } 
    });
}