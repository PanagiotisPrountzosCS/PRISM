#pragma once

#include <cstdint>

namespace PRISM {

using Time = uint64_t;

using RealValue = double;

enum class SensorType { TEMPERATURE, PRESSURE, CO2 };

enum class DataMonitorType { RANDOM, FILE, URL };

enum class ProbabilityDistribution { UNIFORM, NORMAL };

typedef struct RealPoint {
    RealValue x;
    RealValue y;
} RealPoint;

}  // namespace PRISM