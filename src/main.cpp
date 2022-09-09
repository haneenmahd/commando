#include <iostream>
#include "Watcher.hpp"
#include "Config.hpp"
#include "ansi-text/ansi-text.h"

int main(int argc, char* argv[])
{
    // Resolving and reading from the target location
    if (argc < 2)
    {
        std::cout << "You need to specify the location to search for the config file." << std::endl;
        std::cout << std::endl;
        std::cout << "watcher [config_location], " GREEN << "eg: `watcher ./config.json`" << NORMAL << std::endl;

        return EXIT_FAILURE; // exit if there is no target location specified
    }

    const std::string WORKING_DIR = argv[1];

    ConfigReader reader;
    Config config = reader.read(WORKING_DIR);

    std::cout << BLUE << "Building Project: " << COLOR_RESET << BOLD << config.projectName << NORMAL << std::endl;
    std::cout << RED << "Target location: " << COLOR_RESET << config.target << std::endl;
}



// Watcher w{"./", std::chrono::milliseconds(500)};

// w.start([](std::string path_to_watch, WatcherStatus status)
// {
//     if (!std::filesystem::is_regular_file(path_to_watch)) {
//         return;
//     } else {
//         switch (status)
//         {
//         case WatcherStatus::created:
//             system("echo 'Hello `create` from command'");
//             std::cout << "File created: " << path_to_watch << '\n';
//             break;

//         case WatcherStatus::modified:
//             system("echo 'Hello `modify` from command'");
//             std::cout << "File modified: " << path_to_watch << '\n';
//             break;
//         case WatcherStatus::erased:
//             system("echo 'Hello `erase` from command'");
//             std::cout << "File erased: " << path_to_watch << '\n';
//             break;
//         default:
//             std::cout << "Error! Unknown file status.\n";
//         }
//     }
// });