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
        std::cout << "commando [config_location], " GREEN << "eg: `commando ./config.json`" << NORMAL << std::endl;

        return EXIT_FAILURE; // exit if there is no target location specified
    }

    const std::string WORKING_DIR = argv[1];

    ConfigReader reader;
    Config config = reader.read(WORKING_DIR);

    std::cout << BLUE << "Project: " << COLOR_RESET << BOLD << config.projectName << NORMAL << std::endl;
    std::cout << RED << "Target location: " << COLOR_RESET << config.target << std::endl;

    Watcher::startWatching(config);
}