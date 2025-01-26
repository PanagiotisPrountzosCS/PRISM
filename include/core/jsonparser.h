#pragma once

#include <string>
#include <vector>
#include <map>

namespace PRISM{
    class JSONParser{
        public:
        enum Type {Null, Boolean, Number, String, Array, Object};
        class Value{
            public:
            Type type;
            std::string str_val;
            double num_val;
            bool bool_val;
            std::vector<Value> array_val;
            std::map<std::string, Value> object_val;

            Value() : type(Null) {}

            static Value parse(const std::string& json);
            private:
            static Value parseValue(const std::string& json, size_t& pos);
            static Value parseObject(const std::string& json, size_t& pos);
            static Value parseArray(const std::string& json, size_t& pos);
            static Value parseString(const std::string& json, size_t& pos);
            static Value parseNumber(const std::string& json, size_t& pos);
        };
    };
}
