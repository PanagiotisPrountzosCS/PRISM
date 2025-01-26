#include <iostream>

#include "cli/mainapplication.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }
    PRISM_CLI::mainLoop(argv[1]);
    return 0;
}
