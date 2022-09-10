#include <stdlib.h>
#include <string>

class System {
    public:
    // replacing the default `system` function used for running commands
    void spawn(std::string command) {
        if (!command.empty())
        {
            system(command.c_str());
        }
    };
};