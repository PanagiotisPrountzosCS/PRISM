#pragma once

#include <chrono>

namespace PRISM_CLI {

bool shouldRun{true};

constexpr auto pollInterval_ms = std::chrono::milliseconds(100);

constexpr uint32_t threadSleepTime_ms = 10;

constexpr size_t maxMeasurements = 100;

}