#pragma once

#include <cstdint>

namespace PRISM {

using Time = uint64_t;

using RealValue = double;

enum class SensorType { TEMPERATURE, PRESSURE, CO2 };

enum class ProbabilityDistribution { UNIFORM, NORMAL };

}  // namespace PRISM