#pragma once

#include <chrono>

#include "core/jsonparser.h"

namespace PRISM_CLI {

bool shouldRun{true};

constexpr auto pollInterval_ms = std::chrono::milliseconds(100);

constexpr uint32_t threadSleepTime_ms = 80;

constexpr size_t maxMeasurements = 100;

bool validateConfig(const PRISM::JSONParser::Value& config);

PRISM::Sensor createSensor(const PRISM::JSONParser::Value& sensor);

}
