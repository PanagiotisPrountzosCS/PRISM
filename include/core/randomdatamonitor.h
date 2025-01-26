#pragma once

#include <memory>

#include "core/idatamonitor.h"
#include "datagenerators/randomnumberfactory.h"

namespace PRISM {

class RandomDataMonitor : public IDataMonitor {
public:
    RandomDataMonitor(std::shared_ptr<RandomNumberFactory> randomNumberFactory);
    bool poll() override;
    size_t size() const override;
    RealPoint getNextMeasurement() override;

private:
    std::shared_ptr<RandomNumberFactory> _randomNumberFactory;
};

}  // namespace PRISM
