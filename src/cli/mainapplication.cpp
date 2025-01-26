#include "cli/mainapplication.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "cli/helpers.h"
#include "core/jsonparser.h"
#include "core/randomdatamonitor.h"

namespace PRISM_CLI {

bool shouldRun{true};

void pollingCallback(SensorMap& sensors) {
    for (auto& sensor : sensors) {
        std::cout << "============ Polling " << sensor.second.getName() << " ============\n";
        sensor.second.poll();
        std::cout << "======= Done polling " << sensor.second.getName() << " =============\n\n";

        if (sensor.second.size() >= maxMeasurements) {
            std::cout << sensor.second.getName() << '\n';
            for (size_t i = 0; i < maxMeasurements; ++i) {
                std::cout << "(" << sensor.second.getMeasurementByIndex(i).readTime_us << ", "
                          << sensor.second.getMeasurementByIndex(i).value << ")\n";
            }
            // save measurements to file
            // then clear the sensor
            sensor.second.clear();
            sensor.second.freeHeap();
        }
    }
}

void mainLoop(const char* configPath) {
    // set up all the sensors
    SensorMap sensors;

    // parse config. Will throw exception if config cannot be parsed
    PRISM::JSONParser::Value config = parseConfig(std::string(configPath));

    // validate config. Will throw exception if config is invalid
    validateConfig(config);

    // create sensors
    createSensors(config, sensors);

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
}

}  // namespace PRISM_CLI