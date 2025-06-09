#ifndef __SENSOR_H
#define __SENSOR_H

#include <vector>

#include "stdint.h"

namespace PRISM
{
typedef struct measurement
{
        float x;
        float y;
        float z;
        uint32_t timestamp;
} measurement;

typedef struct sensor
{
        uint32_t id;
        std::vector<measurement> data;
} sensor;
}  // namespace PRISM
#endif