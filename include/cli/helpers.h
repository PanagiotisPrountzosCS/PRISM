#pragma once

#include <chrono>

#include "core/jsonparser.h"

namespace PRISM_CLI {

constexpr auto pollInterval_ms = std::chrono::milliseconds(100);

constexpr uint32_t threadSleepTime_ms = 80;

constexpr size_t maxMeasurements = 100;

PRISM::JSONParser::Value parseConfig(const std::string& config);

void validateConfig(const PRISM::JSONParser::Value& config);

void createSensors(const PRISM::JSONParser::Value& config, std::vector<PRISM::Sensor>& sensors);

}
