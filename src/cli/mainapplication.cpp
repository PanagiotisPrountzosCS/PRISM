#include "cli/mainapplication.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include "cli/variables.h"
#include "core/objectid.h"
#include "core/randomdatamonitor.h"
#include "core/sensor.h"
#include "datagenerators/randomnumberfactory.h"
#include "core/jsonparser.h"

namespace PRISM_CLI {

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

void mainLoop() {
    // set up all the sensors
    SensorMap sensors;

    std::ifstream configFile("config.json");
    std::stringstream configBuffer;
    configBuffer << configFile.rdbuf();

    try{
        PRISM::JSONParser::Value config = PRISM::JSONParser::Value::parse(configBuffer.str());

        std::cout << "Config parsed successfully\n";

        if(!validateConfig(config)) {
            std::cerr << "Invalid config file\n";
            return;
        }

        // create the sensors
        for (const auto& sensor : config.array_val) {
            //create sensor here
        }
    }
    catch (const std::exception& e){
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        return;
    }

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
        std::this_thread::sleep_for(std::chrono::milliseconds(threadSleepTime_ms));
    }
}

}  // namespace PRISM_CLI
