#ifndef __SENSOR_H
#define __SENSOR_H

#include <memory>
#include <vector>

#include "coredefs.h"
#include "linear_regression_calc.h"
#include "stdint.h"

namespace PRISM
{

typedef struct sensor
{
        uint32_t id;
        std::shared_ptr<std::vector<measurement>> data;
        std::shared_ptr<linear_regression_calc> predictor;

        sensor(uint32_t sensor_id)
            : id(sensor_id),
              data(std::make_shared<std::vector<measurement>>()),
              predictor(std::make_shared<linear_regression_calc>(data))
        {
        }
} sensor;

}  // namespace PRISM
#endif