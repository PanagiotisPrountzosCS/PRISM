#pragma once

#include <chrono>

namespace PRISM_CLI {

bool shouldRun{true};

constexpr auto pollInterval_ms = std::chrono::milliseconds(5);

constexpr uint32_t threadSleepTime_ms = 0;

constexpr size_t maxMeasurements = 10000;

}