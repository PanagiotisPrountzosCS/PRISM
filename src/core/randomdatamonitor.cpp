#include "core/randomdatamonitor.h"

#include "datagenerators/randomnumberfactory.h"

namespace PRISM {

RandomDataMonitor::RandomDataMonitor(std::shared_ptr<RandomNumberFactory> randomNumberFactory)
    : _randomNumberFactory(randomNumberFactory) {
    _buffer = std::queue<RealPoint>{};
}

bool RandomDataMonitor::poll() {
    RealPoint newMeasurement = {_randomNumberFactory->createRandomNumber(),
                                _randomNumberFactory->createRandomNumber()};
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