#include <stdlib.h>
#include <string>

class Process {
    public:
    // replacing the default `system` function used for running commands
    static void spawn(std::string command) {
        if (!command.empty())
        {
            std::cout << BOLD << "Running 🔨: " << GREEN << command << NORMAL << std::endl;
            std::cout << std::endl;

            system(command.c_str());
            
            std::cout << std::endl;
        }
    };

    static void runCommand(Config config, WatcherStatus status) {
        switch (status)
        {
        case WatcherStatus::created:
            Process::spawn(config.createdCommand);
            break;

        case WatcherStatus::modified:
            Process::spawn(config.modifiedCommand);
            break;
        case WatcherStatus::erased:
            Process::spawn(config.erasedCommand);
            break;
        default:
            std::cout << "Error! Unknown file status.\n";
        }
    }
};