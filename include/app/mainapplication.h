#pragma once

#include <unordered_map>

#include "core/sensor.h"

namespace PRISM {

using SensorMap = std::unordered_map<PRISM::ObjectId, PRISM::Sensor, PRISM::ObjectIdHash>;

void handleSignal(int signal);

std::shared_ptr<SensorMap> initApp(const char* configPath);

void cleanup(SensorMap& sensors);

void pollingCallback(const SensorMap& sensors);

void mainLoop(std::shared_ptr<SensorMap> sensors);

}  // namespace PRISM