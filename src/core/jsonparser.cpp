#include <stdexcept>

#include "core/jsonparser.h"

namespace PRISM{
    JSONParser::Value JSONParser::Value::parse(const std::string& json){
        size_t pos = 0;
        return parseValue(json, pos);
    }

    JSONParser::Value JSONParser::Value::parseValue(const std::string& json, size_t& pos){
        while (pos < json.length() && std::isspace(json[pos])) pos++;

        if (pos >= json.length()) throw std::runtime_error("Unexpected end of input");

        char c = json[pos];
        if (c == '{') return parseObject(json, pos);
        if (c == '[') return parseArray(json, pos);
        if (c == '"') return parseString(json, pos);
        if (c == 't' && json.substr(pos, 4) == "true") {
            pos += 4;
            Value v;
            v.type = Boolean;
            v.bool_val = true;
            return v;
        }
        if (c == 'f' && json.substr(pos, 5) == "false") {
            pos += 5;
            Value v;
            v.type = Boolean;
            v.bool_val = false;
            return v;
        }
        if (c == 'n' && json.substr(pos, 4) == "null") {
            pos += 4;
            return Value();
        }
        return parseNumber(json, pos);  
    }

    JSONParser::Value JSONParser::Value::parseObject(const std::string& json, size_t& pos){
        Value v;
        v.type = Object;
        pos++; // skip '{'

        while (pos < json.length() && json[pos] != '}') {
            while (pos < json.length() && std::isspace(json[pos])) pos++;
            Value key = parseString(json, pos);

            while (pos < json.length() && std::isspace(json[pos])) pos++;
            if (json[pos] != ':') throw std::runtime_error("Expected ':'");
            pos++;

            v.object_val[key.str_val] = parseValue(json, pos);

            while (pos < json.length() && std::isspace(json[pos])) pos++;
            if (json[pos] == ',') pos++;
            else if (json[pos] != '}') throw std::runtime_error("Expected ',' or '}'");
        }

        if (pos < json.length() && json[pos] == '}') pos++;
        return v;
    }

    JSONParser::Value JSONParser::Value::parseArray(const std::string& json, size_t& pos){
        Value v;
        v.type = Array;
        pos++; // skip '['

        while (pos < json.length() && json[pos] != ']') {
            v.array_val.push_back(parseValue(json, pos));
            while (pos < json.length() && std::isspace(json[pos])) pos++;
            if (json[pos] == ',') pos++;
            else if (json[pos] != ']') throw std::runtime_error("Expected ',' or ']'");
        }
        if (pos < json.length() && json[pos] == ']') pos++;
        return v;
    }

    JSONParser::Value JSONParser::Value::parseString(const std::string& json, size_t& pos){
        Value v;
        v.type = String;
        pos++; // skip opening quote

        while (pos < json.length() && json[pos] != '"') {
            if (json[pos] == '\\') {
                pos++;
                if (pos >= json.length()) throw std::runtime_error("Unexpected end in string");
            }
            v.str_val += json[pos];
            pos++;
        }
        if (pos < json.length() && json[pos] == '"') pos++;
        return v;
    }

    JSONParser::Value JSONParser::Value::parseNumber(const std::string& json, size_t& pos){
        Value v;
        v.type = Number;
        std::string num_str;

        while (pos < json.length() && 
               (std::isdigit(json[pos]) || 
                json[pos] == '-' || 
                json[pos] == '.' || 
                json[pos] == 'e' || 
                json[pos] == 'E' || 
                json[pos] == '+')) {    
            num_str += json[pos];
            pos++;
        }
        v.num_val = std::stod(num_str);
        return v;
    }
}