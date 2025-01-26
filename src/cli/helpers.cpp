#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm>

#include "core/jsonparser.h"
#include "cli/helpers.h"


namespace PRISM_CLI {

std::map<std::string, PRISM::DataMonitorType> dataMonitorTypeMap = {
    {"file", PRISM::DataMonitorType::FILE},
    {"random", PRISM::DataMonitorType::RANDOM},
    {"url", PRISM::DataMonitorType::URL}
};

std::map<std::string, PRISM::SensorType> sensorTypeMap = {
    {"co2", PRISM::SensorType::CO2},
    {"pressure", PRISM::SensorType::PRESSURE},
    {"temperature", PRISM::SensorType::TEMPERATURE}
};


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

            if (name == sensor.object_val.end() || name->second.type != PRISM::JSONParser::Type::String ||
                name->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing name");
            }

            if (type == sensor.object_val.end() || type->second.type != PRISM::JSONParser::Type::String ||
                type->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing type");
            }

            if (upperLimit == sensor.object_val.end() || upperLimit->second.type != PRISM::JSONParser::Type::Number) {
                throw std::runtime_error("Sensor is missing upperLimit");
            }

            if (lowerLimit == sensor.object_val.end() || lowerLimit->second.type != PRISM::JSONParser::Type::Number) {
                throw std::runtime_error("Sensor is missing lowerLimit");
            }

            if (isActive == sensor.object_val.end() || isActive->second.type != PRISM::JSONParser::Type::Boolean) {
                throw std::runtime_error("Sensor is missing isActive");
            }

            if (dataMonitor == sensor.object_val.end() || dataMonitor->second.type != PRISM::JSONParser::Type::String ||
                dataMonitor->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing dataMonitor");
            }

            if (unit == sensor.object_val.end() || unit->second.type != PRISM::JSONParser::Type::String ||
                unit->second.str_val.empty()) {
                throw std::runtime_error("Sensor is missing unit");
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error validating config: " << e.what() << std::endl;
        exit(1);
    }

    std::cout << "Config syntax validated successfully\n";
}

void createSensors(const PRISM::JSONParser::Value& config, SensorMap& sensors) {
    try {
        for (const auto& sensor : config.array_val) {
            auto name = sensor.object_val.find("name")->second.str_val;
            auto type = sensor.object_val.find("type")->second.str_val;
            auto upperLimit = sensor.object_val.find("upperLimit")->second.num_val;
            auto lowerLimit = sensor.object_val.find("lowerLimit")->second.num_val;
            auto isActive = sensor.object_val.find("isActive")->second.bool_val;
            auto dataMonitor = sensor.object_val.find("dataMonitor")->second.str_val;
            auto unit = sensor.object_val.find("unit")->second.str_val;

            //convert dataMonitor to lowercase then to enum
            //same for type
            std::transform(dataMonitor.begin(), dataMonitor.end(), dataMonitor.begin(), [](unsigned char c){ return std::tolower(c); });

            std::transform(type.begin(), type.end(), type.begin(), [](unsigned char c){ return std::tolower(c); });

            if (isActive) {
                if(upperLimit <= lowerLimit) {
                    throw std::runtime_error("Upper limit is less than lower limit");
                }

                if(dataMonitorTypeMap.find(dataMonitor) == dataMonitorTypeMap.end()) {
                    throw std::runtime_error("Invalid data monitor");
                }

                if(sensorTypeMap.find(type) == sensorTypeMap.end()) {
                    throw std::runtime_error("Invalid sensor type");
                }
                auto dataMonitorEnum = dataMonitorTypeMap[dataMonitor];
                auto typeEnum = sensorTypeMap[type];

                PRISM::Sensor newSensor(name, typeEnum, dataMonitorEnum, upperLimit, lowerLimit, unit);

                sensors.emplace(newSensor.getId(), newSensor);

                std::cout << "Sensor " << name << " is active\n";
            }
            else {
                std::cout << "Sensor " << name << " is not active\n";
            }
        }
        std::cout << "Sensors created successfully\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error creating sensors: " << e.what() << std::endl;
        exit(1);
    }
}

}  // namespace PRISM_CLI