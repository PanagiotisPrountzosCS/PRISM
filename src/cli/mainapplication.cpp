#include "cli/mainapplication.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "cli/variables.h"
#include "core/objectid.h"
#include "core/randomdatamonitor.h"
#include "core/sensor.h"
#include "datagenerators/randomnumberfactory.h"

namespace PRISM_CLI {

void pollingCallback(SensorMap& sensors) {
    for (auto& sensor : sensors) {
        std::cout << "============ Polling " << sensor.second.getName() << " ============\n";
        sensor.second.poll();
        std::cout << "======= Done polling " << sensor.second.getName() << " =============\n\n";

        if (sensor.second.size() >= maxMeasurements) {
            for (size_t i = 0; i < maxMeasurements; ++i) {
                std::cout << "(" << sensor.second.getMeasurementByIndex(i).readTime_us << ", "
                          << sensor.second.getMeasurementByIndex(i).value << ")\n";
            }
            // save measurements to file
            // then clear the sensor
            sensor.second.clear();
        }
    }
}

void mainLoop() {
    // set up all the sensors
    SensorMap sensors;

    PRISM::Sensor sensor1("Temperature Sensor #1", PRISM::SensorType::TEMPERATURE,
                          std::make_shared<PRISM::RandomDataMonitor>(
                              std::make_shared<PRISM::RandomNumberFactory>(
                                  50, 100, PRISM::ProbabilityDistribution::NORMAL)));
    sensors.emplace(sensor1.getId(), sensor1);
    PRISM::Sensor sensor2("Pressure Sensor #2", PRISM::SensorType::PRESSURE,
                          std::make_shared<PRISM::RandomDataMonitor>(
                              std::make_shared<PRISM::RandomNumberFactory>(
                                  100, 200, PRISM::ProbabilityDistribution::NORMAL)));
    sensors.emplace(sensor2.getId(), sensor2);

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
