#include "cli/helpers.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "core/jsonparser.h"

namespace PRISM_CLI {

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

void createSensors(const PRISM::JSONParser::Value& config, std::vector<PRISM::Sensor>& sensors) {

}

}  // namespace PRISM_CLI