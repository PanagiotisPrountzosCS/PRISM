#pragma once

#include <cstdint>

#include "globaldefinitions.h"
#include "objectid.h"

namespace PRISM {

struct Measurement {
    Time timestamp_us;    // Unix timestamp in microseconds
    Time readTime_us;     // When measurement was read
    RealValue value;      // Measurement value
    ObjectId sensorId;    // Sensor identifier
    uint32_t errorFlags;  // For potential error conditions
};

}  // namespace PRISM