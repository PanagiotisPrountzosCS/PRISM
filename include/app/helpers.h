#pragma once

#include <chrono>
#include <unordered_map>

#include "core/jsonparser.h"
#include "core/sensor.h"

namespace PRISM {

extern bool shouldRun;

using SensorMap = std::unordered_map<PRISM::ObjectId, PRISM::Sensor, PRISM::ObjectIdHash>;

constexpr auto pollInterval_ms = std::chrono::milliseconds(500);

constexpr auto threadSleepTime_ms = std::chrono::milliseconds(10);

extern double appStartTime_ms;

constexpr size_t maxMeasurements = 100;

constexpr int chartRangePadding = 5;

JSONParser::Value parseConfig(const std::string& config);

void validateConfig(const PRISM::JSONParser::Value& config);

std::shared_ptr<SensorMap> createSensors(const PRISM::JSONParser::Value& config);

}  // namespace PRISM
