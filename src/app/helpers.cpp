#include "app/helpers.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "core/jsonparser.h"

namespace PRISM {

PRISM::JSONParser::Value parseConfig(const std::string& config) {
    std::ifstream configFile(config);
    std::stringstream configBuffer;
    configBuffer << configFile.rdbuf();

    try {
        // might throw an exception
        PRISM::JSONParser::Value config = PRISM::JSONParser::Value::parse(configBuffer.str());
        std::cout << "Config parsed successfully\n";
        return config;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing config file: " << e.what() << std::endl;
        exit(1);
    }
}

void validateConfig(const PRISM::JSONParser::Value& config) {
    try {
        if (config.type != PRISM::JSONParser::Type::Array) {
            throw std::runtime_error("Config is not an array");
        }
        for (const auto& sensor : config.array_val) {
            if (sensor.type != PRISM::JSONParser::Type::Object) {
                throw std::runtime_error("Sensor is not an object");
            }

            auto name = sensor.object_val.find("name");
            auto type = sensor.object_val.find("type");
            auto upperLimit = sensor.object_val.find("upperLimit");
            auto lowerLimit = sensor.object_val.find("lowerLimit");
            auto isActive = sensor.object_val.find("isActive");
            auto dataMonitor = sensor.object_val.find("dataMonitor");
            auto unit = sensor.object_val.find("unit");

            if (name == sensor.object_val.end() ||
                name->second.type != PRISM::JSONParser::Type::String ||
                name->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing name");
            }

            if (type == sensor.object_val.end() ||
                type->second.type != PRISM::JSONParser::Type::String ||
                type->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing type");
            }

            if (upperLimit == sensor.object_val.end() ||
                upperLimit->second.type != PRISM::JSONParser::Type::Number) {
                throw std::runtime_error("Sensor is missing upperLimit");
            }

            if (lowerLimit == sensor.object_val.end() ||
                lowerLimit->second.type != PRISM::JSONParser::Type::Number) {
                throw std::runtime_error("Sensor is missing lowerLimit");
            }

            if (isActive == sensor.object_val.end() ||
                isActive->second.type != PRISM::JSONParser::Type::Boolean) {
                throw std::runtime_error("Sensor is missing isActive");
            }

            if (dataMonitor == sensor.object_val.end() ||
                dataMonitor->second.type != PRISM::JSONParser::Type::String ||
                dataMonitor->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing dataMonitor");
            }

            if (unit == sensor.object_val.end() ||
                unit->second.type != PRISM::JSONParser::Type::String ||
                unit->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing unit");
            }
            if (upperLimit->second.num_val <= lowerLimit->second.num_val) {
                throw std::runtime_error("Upper limit is less than lower limit");
            }

            // convert dataMonitor to lowercase then to enum
            // same for type
            std::string dataMonitorStr = dataMonitor->second.str_val;
            std::string typeStr = type->second.str_val;

            std::transform(dataMonitorStr.begin(), dataMonitorStr.end(), dataMonitorStr.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            std::transform(typeStr.begin(), typeStr.end(), typeStr.begin(),
                           [](unsigned char c) { return std::tolower(c); });

            if (PRISM::stringToDataMonitorType.find(dataMonitorStr) ==
                PRISM::stringToDataMonitorType.end()) {
                throw std::runtime_error("Invalid data monitor");
            }

            if (PRISM::stringToSensorType.find(typeStr) == PRISM::stringToSensorType.end()) {
                throw std::runtime_error("Invalid sensor type");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error validating config: " << e.what() << std::endl;
        exit(1);
    }

    std::cout << "Config syntax validated successfully\n";
}

std::shared_ptr<SensorMap> createSensors(const PRISM::JSONParser::Value& config) {
    size_t sensorCount = 0;
    std::shared_ptr<SensorMap> sensors = std::make_shared<SensorMap>();
    try {
        for (const auto& sensor : config.array_val) {
            auto name = sensor.object_val.find("name")->second.str_val;
            auto type = sensor.object_val.find("type")->second.str_val;
            auto upperLimit = sensor.object_val.find("upperLimit")->second.num_val;
            auto lowerLimit = sensor.object_val.find("lowerLimit")->second.num_val;
            auto isActive = sensor.object_val.find("isActive")->second.bool_val;
            auto dataMonitor = sensor.object_val.find("dataMonitor")->second.str_val;
            auto unit = sensor.object_val.find("unit")->second.str_val;

            if (isActive) {
                auto dataMonitorEnum = PRISM::stringToDataMonitorType[dataMonitor];
                auto typeEnum = PRISM::stringToSensorType[type];

                PRISM::Sensor newSensor(name, typeEnum, dataMonitorEnum, lowerLimit, upperLimit,
                                        unit);

                sensors->emplace(newSensor.getId(), newSensor);

                sensorCount++;

                std::cout << "Sensor " << name << " is active\n";
            } else {
                std::cout << "Sensor " << name << " is not active\n";
            }
        }
        std::cout << sensorCount << " sensors created successfully\n";
    } catch (const std::exception& e) {
        std::cerr << "Error creating sensors: " << e.what() << std::endl;
        exit(1);
    }
    return sensors;
}

}  // namespace PRISM