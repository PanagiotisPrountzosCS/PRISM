#include "cli/mainapplication.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_map>

#include "cli/variables.h"
#include "core/objectid.h"
#include "core/sensor.h"

namespace PRISM_CLI {

void pollingCallback(SensorMap& sensors) {
    for (auto& sensor : sensors) {
        sensor.second.poll();
        if (sensor.second.size() >= maxMeasurements) {
            // save measurements to file
            // then clear the sensor
            sensor.second.clear();
        }
    }
}

void mainLoop() {
    // set up all the sensors
    SensorMap sensors;

    PRISM::Sensor sensor1("Temperature Sensor #1", PRISM::SensorType::TEMPERATURE);
    sensors.emplace(sensor1.getId(), sensor1);
    PRISM::Sensor sensor2("Pressure Sensor #2", PRISM::SensorType::PRESSURE);
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
