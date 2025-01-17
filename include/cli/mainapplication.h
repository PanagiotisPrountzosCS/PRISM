#pragma once
#include <unordered_map>

#include "core/objectid.h"
#include "core/sensor.h"

namespace PRISM_CLI {

using SensorMap = std::unordered_map<PRISM::ObjectId, PRISM::Sensor, PRISM::ObjectIdHash>;

void mainLoop();

void pollingCallback(const SensorMap& sensors);

}  // namespace PRISM_CLI