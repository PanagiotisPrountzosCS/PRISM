#include "core/sensor.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "core/randomdatamonitor.h"

namespace PRISM {

std::map<std::string, PRISM::DataMonitorType> stringToDataMonitorType = {
    {"file", PRISM::DataMonitorType::FILE},
    {"random", PRISM::DataMonitorType::RANDOM},
    {"url", PRISM::DataMonitorType::URL}};

std::map<std::string, PRISM::SensorType> stringToSensorType = {
    {"co2", PRISM::SensorType::CO2},
    {"pressure", PRISM::SensorType::PRESSURE},
    {"temperature", PRISM::SensorType::TEMPERATURE}};

std::map<PRISM::DataMonitorType, std::string> dataMonitorTypeToString = {
    {PRISM::DataMonitorType::FILE, "file"},
    {PRISM::DataMonitorType::RANDOM, "random"},
    {PRISM::DataMonitorType::URL, "url"}};

std::map<PRISM::SensorType, std::string> sensorTypeToString = {
    {PRISM::SensorType::CO2, "co2"},
    {PRISM::SensorType::PRESSURE, "pressure"},
    {PRISM::SensorType::TEMPERATURE, "temperature"}};

Sensor::Sensor(std::string name, SensorType type, DataMonitorType dataMonitorType,
               RealValue upperLimit, RealValue lowerLimit, std::string unit)
    : _id(),
      _name(std::move(name)),
      _type(type),
      _dataMonitorType(dataMonitorType),
      _upperLimit(upperLimit),
      _lowerLimit(lowerLimit),
      _unit(unit) {
    if (dataMonitorType == DataMonitorType::FILE) {
        // TODO: Implement file data monitor
    } else if (dataMonitorType == DataMonitorType::RANDOM) {
        auto randomNumberFactory = std::make_shared<RandomNumberFactory>(
            lowerLimit, upperLimit, ProbabilityDistribution::NORMAL);
        _dataMonitor = std::make_shared<RandomDataMonitor>(randomNumberFactory);
    } else if (dataMonitorType == DataMonitorType::URL) {
        // TODO: Implement URL data monitor
    } else {
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

void Sensor::saveMeasurements() {
    // Get the current time as a string
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    std::tm* tm_now = std::localtime(&time_t_now);
    
    // Create a unique filename using the current time
    std::ostringstream filename;
    filename << "prism_log_"
             << std::put_time(tm_now, "%Y%m%d_%H%M%S") // Format: YYYYMMDD_HHMMSS
             << ".txt";

    std::ofstream file(filename.str(), std::ios::app);
    file << "{\nSensor: " << _name << "\n";
    file << "Type: " << sensorTypeToString[_type] << "\n";
    file << "Unit: " << _unit << "\n";
    file << "Upper Limit: " << _upperLimit << "\n";
    file << "Lower Limit: " << _lowerLimit << "\n";
    file << "Data Monitor Type: " << dataMonitorTypeToString[_dataMonitorType] << "\n";
    file << "Measurements: \n";
    for (const auto& measurement : _measurements) {
        file << measurement.readTime_us << ", " << measurement.timestamp_us << ", " << measurement.value << "\n";
    }
    file << "}\n";
}

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
