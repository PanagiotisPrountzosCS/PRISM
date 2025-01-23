#pragma once

#include <memory>  // Include for smart pointers
#include <string>
#include <vector>

#include "core/idatamonitor.h"
#include "globaldefinitions.h"
#include "measurement.h"
#include "objectid.h"

namespace PRISM {

class Sensor {
public:
    Sensor(std::string name, SensorType type,
           std::shared_ptr<IDataMonitor> dataMonitor);  // Creates sensor with new unique ID

    // Getters
    const std::string& getName() const;
    ObjectId getId() const;
    double getLastValue() const;
    int64_t getLastTimestamp() const;
    SensorType getType() const;

    Measurement createMeasurement(RealValue value, Time timestamp_us);
    Measurement getMeasurementByIndex(size_t index) const;
    Time getXByIndex(size_t index) const;
    RealValue getYByIndex(size_t index) const;
    void appendMeasurement(Measurement m);
    void clear();
    void poll();
    size_t size() const;

private:
    ObjectId _id;
    std::string _name;
    SensorType _type;
    double _lastValue{0.0};
    int64_t _lastTimestamp{0};
    std::vector<Measurement> _measurements;
    std::shared_ptr<IDataMonitor> _dataMonitor;  // Use smart pointer
};

}  // namespace PRISM