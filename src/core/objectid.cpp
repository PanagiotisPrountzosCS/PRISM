#include "core/objectid.h"

#include <sstream>

namespace PRISM {

uint64_t ObjectId::_nextId = 1;  // Start from 1, 0 can be reserved for invalid/null

ObjectId::ObjectId() : _id(_nextId++) {}

ObjectId::ObjectId(uint64_t value) : _id(value) {}

uint64_t ObjectId::getValue() const { return _id; }

std::string ObjectId::toString() const {
    std::stringstream ss;
    ss << "0x" << std::hex << _id;
    return ss.str();
}

bool ObjectId::isValid() const { return _id != 0; }

bool ObjectId::operator==(const ObjectId& other) const { return _id == other._id; }

bool ObjectId::operator!=(const ObjectId& other) const { return _id != other._id; }

size_t ObjectIdHash::operator()(const ObjectId& id) const {
    return std::hash<uint64_t>()(id.getValue());
}

}  // namespace PRISM