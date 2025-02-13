#pragma once

#include <cstdint>

#include "globaldefinitions.h"
#include "objectid.h"

namespace PRISM {

typedef struct Measurement {
    RealValue timestamp_us;  // Unix timestamp in microseconds     (x value)
    Time readTime_us;   // When measurement was read          (time read by PRISM)
    RealValue value;    // Measurement value                  (y value)
    ObjectId sensorId;  // Sensor identifier
} Measurement;

}  // namespace PRISM