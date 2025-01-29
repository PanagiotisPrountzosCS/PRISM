#include <iostream>

#include "app/helpers.h"
#include "app/mainapplication.h"

double PRISM::appStartTime_ms;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <config_path>" << std::endl;
        return 1;
    }
    PRISM::appStartTime_ms =
        static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::high_resolution_clock::now().time_since_epoch())
                                .count());
    auto sensors = PRISM::initApp(argv[1]);
    PRISM::mainLoop(sensors);
    return 0;
}
