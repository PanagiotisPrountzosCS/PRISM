#pragma once

#include <any>
#include <random>
#include <variant>

#include "globaldefinitions.h"

namespace PRISM {

class RandomNumberFactory {
public:
    RandomNumberFactory(RealValue lowerLimit, RealValue upperLimit,
                        ProbabilityDistribution distribution);
    RealValue createRandomNumber();
    void setUpperLimit(RealValue upperLimit);
    void setLowerLimit(RealValue lowerLimit);
    void setProbabilityDistribution(ProbabilityDistribution);
    RealValue getUpperLimit();
    RealValue getLowerLimit();
    ProbabilityDistribution getProbabilityDistribution();

private:
    void update();
    std::variant<std::uniform_int_distribution<int64_t>, std::normal_distribution<double>>
        _distribution;
    std::mt19937_64 _randomNumberGenerator;
    ProbabilityDistribution _probabilityDistribution;
    RealValue _upperLimit;
    RealValue _lowerLimit;
};

}  // namespace PRISM