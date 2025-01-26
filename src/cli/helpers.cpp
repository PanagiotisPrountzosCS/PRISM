#include "cli/helpers.h"

namespace PRISM_CLI {

bool validateConfig(const PRISM::JSONParser::Value& config) {
    if(config.type != PRISM::JSONParser::Value::Type::Array) {
        return false;
    }
    for(const auto& sensor : config.array_val) {
        if(sensor.type != PRISM::JSONParser::Value::Type::Object) {
            return false;
        }

        if(sensor.object_val["name"].type != PRISM::JSONParser::Value::Type::String) {
            return false;
        }

        if(sensor.object_val["type"].type != PRISM::JSONParser::Value::Type::String) {
            return false;
        }

        if(sensor.object_val["upperLimit"].type != PRISM::JSONParser::Value::Type::Number) {
            return false;
        }   

        if(sensor.object_val["lowerLimit"].type != PRISM::JSONParser::Value::Type::Number) {
            return false;
        }   

        if(sensor.object_val["isActive"].type != PRISM::JSONParser::Value::Type::Boolean) {
            return false;
        }   

        if(sensor.object_val["dataMonitor"].type != PRISM::JSONParser::Value::Type::String) {
            return false;
        }   

        if(sensor.object_val["unit"].type != PRISM::JSONParser::Value::Type::String) {
            return false;
        }       
    }
    return true;
}

PRISM::Sensor createSensor(const PRISM::JSONParser::Value& sensor) {
    return PRISM::Sensor(sensor.object_val["name"].str_val, sensor.object_val["type"].str_val, sensor.object_val["upperLimit"].num_val, sensor.object_val["lowerLimit"].num_val, sensor.object_val["isActive"].bool_val, sensor.object_val["dataMonitor"].str_val, sensor.object_val["unit"].str_val);
}

}