#pragma once

#include <cstdint>
#include <string>

namespace PRISM {

class ObjectId {
public:
    ObjectId();                         // Generates new unique ID
    explicit ObjectId(uint64_t value);  // For special cases

    uint64_t getValue() const;
    std::string toString() const;
    bool isValid() const;

    bool operator==(const ObjectId& other) const;
    bool operator!=(const ObjectId& other) const;
    bool operator<(const ObjectId& other) const;

private:
    uint64_t _id;
    static uint64_t _nextId;  // Counter for unique IDs
};

class ObjectIdHash {
public:
    size_t operator()(const ObjectId& id) const;
};

}  // namespace PRISM
