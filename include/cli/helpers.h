#pragma once

#include <chrono>
#include <unordered_map>

#include "core/jsonparser.h"
#include "core/sensor.h"

namespace PRISM_CLI {

using SensorMap = std::unordered_map<PRISM::ObjectId, PRISM::Sensor, PRISM::ObjectIdHash>;

constexpr auto pollInterval_ms = std::chrono::milliseconds(10);

constexpr uint32_t threadSleepTime_ms = 9;

constexpr size_t maxMeasurements = 1000;

PRISM::JSONParser::Value parseConfig(const std::string& config);

void validateConfig(const PRISM::JSONParser::Value& config);

void createSensors(const PRISM::JSONParser::Value& config, SensorMap& sensors);

}
