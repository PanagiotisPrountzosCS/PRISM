#pragma once

#include <cstddef>
#include <queue>

#include "globaldefinitions.h"

namespace PRISM {

class Sensor;

class IDataMonitor {
public:
    virtual bool poll() = 0;  // returns true if new data is available, false otherwise
    virtual ~IDataMonitor() = default;
    virtual size_t size() const = 0;
    virtual RealPoint getNextMeasurement() = 0;

protected:
    std::queue<RealPoint> _buffer;
};

}  // namespace PRISM