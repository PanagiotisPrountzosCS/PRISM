#include <chrono>
#include <iostream>

#include "core/randomdatamonitor.h"
#include "core/sensor.h"

namespace PRISM {

Sensor::Sensor(std::string name, SensorType type, DataMonitorType dataMonitorType, RealValue upperLimit, RealValue lowerLimit, std::string unit )
    : _id(), _name(std::move(name)), _type(type), _dataMonitorType(dataMonitorType), _upperLimit(upperLimit), _lowerLimit(lowerLimit), _unit(unit) {
        if (dataMonitorType == DataMonitorType::FILE) {
            // TODO: Implement file data monitor
        } 
        else if (dataMonitorType == DataMonitorType::RANDOM) {
            auto randomNumberFactory = std::make_shared<RandomNumberFactory>(lowerLimit, upperLimit, ProbabilityDistribution::NORMAL);
            _dataMonitor = std::make_shared<RandomDataMonitor>(randomNumberFactory);
        }
        else if (dataMonitorType == DataMonitorType::URL) {
            // TODO: Implement URL data monitor
        }
        else {
            std::cerr << "Invalid data monitor type\n";
            exit(1);
        }
    }

const std::string& Sensor::getName() const { return _name; }

ObjectId Sensor::getId() const { return _id; }

PRISM::RealValue Sensor::getLastValue() const { return _lastValue; }

PRISM::Time Sensor::getLastTimestamp() const { return _lastTimestamp; }

Measurement Sensor::createMeasurement(RealValue value, Time timestamp_us) {
    Measurement m;
    m.timestamp_us = timestamp_us;
    m.value = value;
    m.sensorId = _id;
    auto microSecondReadTime = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch());
    m.readTime_us = microSecondReadTime.count();

    _lastValue = value;
    _lastTimestamp = timestamp_us;

    return m;
}

Measurement Sensor::getMeasurementByIndex(size_t index) const {
    if (index >= _measurements.size()) {
        std::cerr << "Index out of range\n";
        exit(1);
    }
    return _measurements[index];
}

Time Sensor::getXByIndex(size_t index) const {
    if (index >= _measurements.size()) {
        std::cerr << "Index out of range\n";
        exit(1);
    }
    return _measurements[index].timestamp_us;
}

RealValue Sensor::getYByIndex(size_t index) const {
    if (index >= _measurements.size()) {
        std::cerr << "Index out of range\n";
        exit(1);
    }
    return _measurements[index].value;
}

void Sensor::appendMeasurement(Measurement m) { _measurements.push_back(m); }

void Sensor::clear() { _measurements.clear(); }

void Sensor::freeHeap() { _measurements.shrink_to_fit(); }

void Sensor::poll() {
    if (_dataMonitor->poll()) {
        while (_dataMonitor->size() > 0) {
            auto nextPoint = _dataMonitor->getNextMeasurement();
            auto newMeasurement = createMeasurement(nextPoint.x, nextPoint.y);
            std::cout << newMeasurement.readTime_us << " -> (" << newMeasurement.timestamp_us
                      << ", " << newMeasurement.value << ")\n";
            appendMeasurement(newMeasurement);
        }
    }
}

size_t Sensor::size() const { return _measurements.size(); }

}  // namespace PRISM
