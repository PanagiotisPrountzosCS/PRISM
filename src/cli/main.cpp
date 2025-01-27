#include <iostream>

#include "app/mainapplication.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }
    auto sensors = PRISM::initApp(argv[1]);
    PRISM::mainLoop(sensors);
    return 0;
}
