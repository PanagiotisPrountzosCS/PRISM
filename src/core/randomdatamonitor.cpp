#include "core/randomdatamonitor.h"

#include <chrono>
#include <iostream>

#include "app/helpers.h"

namespace PRISM {

RandomDataMonitor::RandomDataMonitor(std::shared_ptr<RandomNumberFactory> randomNumberFactory)
    : _randomNumberFactory(randomNumberFactory) {
    _buffer = std::queue<RealPoint>{};
}

bool RandomDataMonitor::poll() {
    double now = std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::high_resolution_clock::now().time_since_epoch())
                     .count();
    double x = (now - appStartTime_ms) * 1.0 / 1000;
    RealPoint newMeasurement = {x, _randomNumberFactory->createRandomNumber()};
    _buffer.push(newMeasurement);
    return true;
}

size_t RandomDataMonitor::size() const { return _buffer.size(); }

RealPoint RandomDataMonitor::getNextMeasurement() {
    RealPoint measurement = _buffer.front();
    _buffer.pop();
    return measurement;
}

}  // namespace PRISM