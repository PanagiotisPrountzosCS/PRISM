#include "cli/mainapplication.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "cli/helpers.h"
#include "core/jsonparser.h"
#include "core/randomdatamonitor.h"

namespace PRISM_CLI {

void pollingCallback(SensorMap& sensors) {
    for (auto& [id, sensor] : sensors) {
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

void mainLoop(const char* configPath) {
    // set up all the sensors
    SensorMap sensors;

    // parse config. Will throw exception if config cannot be parsed
    PRISM::JSONParser::Value config = parseConfig(std::string(configPath));

    // validate config. Will throw exception if config is invalid
    validateConfig(config);

    // create sensors
    createSensors(config, sensors);

    bool shouldRun{true};
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