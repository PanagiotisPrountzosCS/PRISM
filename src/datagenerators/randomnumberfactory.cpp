#include <memory>
#include <random>
#include <iostream>

#include "datagenerators/randomnumberfactory.h"


namespace PRISM {

RandomNumberFactory::RandomNumberFactory(RealValue lowerLimit, RealValue upperLimit,
                                         ProbabilityDistribution distribution) {
    _probabilityDistribution = distribution;
    _upperLimit = upperLimit;
    _lowerLimit = lowerLimit;
    update();
}

void PRISM::RandomNumberFactory::update() {
    if(_lowerLimit > _upperLimit) {
        std::cerr << "Lower limit must be less than upper limit\n";
        exit(1);
    }
    std::random_device rd{};
    _randomNumberGenerator = std::mt19937_64{rd()};
    // chose a distribution afterwards;
    switch (_probabilityDistribution) {
        case ProbabilityDistribution::UNIFORM:
            _distribution =
                std::uniform_int_distribution((int64_t)_lowerLimit, (int64_t)_upperLimit);
            break;

        case ProbabilityDistribution::NORMAL:
            double mean = (_lowerLimit + _upperLimit) / 2;
            double stdDeviation = (_upperLimit - _lowerLimit) / 6;
            _distribution = std::normal_distribution(mean, stdDeviation);
    }
}

RealValue PRISM::RandomNumberFactory::createRandomNumber() {
    return std::visit(
        [&](auto&& dist) {
            auto result = dist(_randomNumberGenerator);
            return static_cast<RealValue>(result);
        },
        _distribution);
}

RealValue PRISM::RandomNumberFactory::getUpperLimit() { return _upperLimit; }

RealValue PRISM::RandomNumberFactory::getLowerLimit() { return _lowerLimit; }

ProbabilityDistribution PRISM::RandomNumberFactory::getProbabilityDistribution() {
    return _probabilityDistribution;
}

void PRISM::RandomNumberFactory::setUpperLimit(RealValue upperLimit) {
    _upperLimit = upperLimit;
    update();
}

void PRISM::RandomNumberFactory::setLowerLimit(RealValue lowerLimit) {
    _lowerLimit = lowerLimit;
    update();
}

void PRISM::RandomNumberFactory::setProbabilityDistribution(
    ProbabilityDistribution probabilityDistribution) {
    _probabilityDistribution = probabilityDistribution;
    update();
}

}  // namespace PRISM