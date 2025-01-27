#include "app/mainapplication.h"

#include <chrono>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "app/helpers.h"
#include "core/jsonparser.h"
#include "core/randomdatamonitor.h"

namespace PRISM {

bool shouldRun{true};

void sigintHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\nCaught SIGINT (Ctrl + C). Exiting gracefully...\n";
        shouldRun = false;
    }
}

std::shared_ptr<SensorMap> initApp(const char* configPath) {
    // Set up the signal handler
    std::signal(SIGINT, sigintHandler);

    // parse config. Will throw exception if config cannot be parsed
    PRISM::JSONParser::Value config = parseConfig(std::string(configPath));

    // validate config. Will throw exception if config is invalid
    validateConfig(config);
    // create sensors
    auto sensors = createSensors(config);
    return sensors;
}

void cleanup(std::shared_ptr<SensorMap> sensors) {
    std::cout << "Cleaning up resources...\n";
    for (auto& [id, sensor] : *sensors) {
        sensor.saveMeasurements();
        sensor.clear();
        sensor.freeHeap();
    }
}

void pollingCallback(std::shared_ptr<SensorMap> sensors) {
    for (auto& [id, sensor] : *sensors) {
        std::cout << "============ Polling " << sensor.getName() << " ============\n";
        sensor.poll();
        std::cout << "======= Done polling " << sensor.getName() << " =============\n\n";

        if (sensor.size() >= maxMeasurements) {
            sensor.saveMeasurements();
            sensor.clear();
            sensor.freeHeap();
        }
    }
}

void mainLoop(std::shared_ptr<SensorMap> sensors) {
    // main loop
    auto startTime = std::chrono::high_resolution_clock::now();

    while (shouldRun) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);

        if (elapsed > pollInterval_ms) {
            pollingCallback(sensors);  // poll
            startTime = currentTime;   // reset the timer
        }
        std::this_thread::sleep_for(threadSleepTime_ms);
    }

    cleanup(sensors);
}

}  // namespace PRISM
