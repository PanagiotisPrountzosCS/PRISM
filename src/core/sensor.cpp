#include "core/sensor.h"

#include <chrono>
#include <iostream>

#include "datagenerators/randomnumberfactory.h"

namespace PRISM {

Sensor::Sensor(std::string name, SensorType type)
    : _id(), _name(std::move(name)), _type(type) {}  // Initialize sensor type

const std::string& Sensor::getName() const { return _name; }

ObjectId Sensor::getId() const { return _id; }

double Sensor::getLastValue() const { return _lastValue; }

int64_t Sensor::getLastTimestamp() const { return _lastTimestamp; }

Measurement Sensor::createMeasurement(RealValue value, Time timestamp_us) {
    Measurement m;
    m.timestamp_us = timestamp_us;
    m.value = value;
    m.sensorId = _id;
    m.readTime_us = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    m.errorFlags = 0;

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
void Sensor::poll() {
    // if(existsNewMeasurement){
    //   appendMeasurement(createMeasurement(...));
    // }

    auto randomNumberFactory = RandomNumberFactory(50, 100, ProbabilityDistribution::NORMAL);

    auto newMeasurement = createMeasurement(randomNumberFactory.createRandomNumber(),
                                            randomNumberFactory.createRandomNumber());
    appendMeasurement(newMeasurement);
    std::cout << _name << " contains (" << newMeasurement.timestamp_us << ", "
              << newMeasurement.value << ")\n";
}

size_t Sensor::size() const { return _measurements.size(); }

}  // namespace PRISM
