#ifndef __LINEAR_REGRESSION_CALC_H
#define __LINEAR_REGRESSION_CALC_H

#include <stddef.h>

#include <memory>
#include <vector>

#include "coredefs.h"

namespace
{
constexpr size_t max_rolling_window_size = 100;
}

namespace PRISM
{

typedef struct linear_regression_calc
{
        linear_regression_calc(std::shared_ptr<std::vector<measurement>> data)
            : data(data)
        {
        }

        std::vector<float> predict_n_x(size_t n)
        {
                std::vector<float> predictions;
                for (size_t i = 1; i <= n; i++)
                {
                        predictions.push_back(get_x_a() * (window_stop + i) +
                                              get_x_b());
                }
                return predictions;
        }
        std::vector<float> predict_n_y(size_t n)
        {
                std::vector<float> predictions;
                for (size_t i = 1; i <= n; i++)
                {
                        predictions.push_back(get_y_a() * (window_stop + i) +
                                              get_y_b());
                }
                return predictions;
        }
        std::vector<float> predict_n_z(size_t n)
        {
                std::vector<float> predictions;
                for (size_t i = 1; i <= n; i++)
                {
                        predictions.push_back(get_z_a() * (window_stop + i) +
                                              get_z_b());
                }
                return predictions;
        }

        float get_x_a()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_x * sum_tt - sum_t * sum_xt) / d;
        }
        float get_y_a()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_y * sum_tt - sum_t * sum_yt) / d;
        }
        float get_z_a()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_z * sum_tt - sum_t * sum_zt) / d;
        }
        float get_x_b()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_xt * window_size - sum_x * sum_t) / d;
        }
        float get_y_b()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_yt * window_size - sum_y * sum_t) / d;
        }

        float get_z_b()
        {
                float d = window_size * sum_tt - sum_t * sum_t;
                if (!d) return 0;
                return (sum_zt * window_size - sum_z * sum_t) / d;
        }

        void clear()
        {
                window_start = 0;
                window_stop = -1;
                sum_t = sum_tt = 0;
                sum_x = sum_xt = 0;
                sum_y = sum_yt = 0;
                sum_z = sum_zt = 0;
        }

        void step_window()
        {
                if (window_size == max_rolling_window_size)
                {
                        sum_t -= data->at(window_start).timestamp;
                        sum_tt -= data->at(window_start).timestamp *
                                  data->at(window_start).timestamp;
                        sum_x -= data->at(window_start).x;
                        sum_xt -= data->at(window_start).x *
                                  data->at(window_start).timestamp;
                        sum_y -= data->at(window_start).y;
                        sum_yt -= data->at(window_start).y *
                                  data->at(window_start).timestamp;
                        sum_z -= data->at(window_start).z;
                        sum_zt -= data->at(window_start).z *
                                  data->at(window_start).timestamp;
                        window_start++;
                }
                else
                {
                        window_size++;
                }
                window_stop++;
                sum_t += data->at(window_stop).timestamp;
                sum_tt += data->at(window_stop).timestamp *
                          data->at(window_stop).timestamp;
                sum_x += data->at(window_stop).x;
                sum_xt +=
                    data->at(window_stop).x * data->at(window_stop).timestamp;
                sum_y += data->at(window_stop).y;
                sum_yt +=
                    data->at(window_stop).y * data->at(window_stop).timestamp;
                sum_z += data->at(window_stop).z;
                sum_zt +=
                    data->at(window_stop).z * data->at(window_stop).timestamp;
        }

        std::shared_ptr<std::vector<measurement>> data;
        int window_start = 0;
        int window_stop = -1;
        int window_size = 0;
        float sum_t = 0;
        float sum_tt = 0;
        float sum_x = 0;
        float sum_xt = 0;
        float sum_y = 0;
        float sum_yt = 0;
        float sum_z = 0;
        float sum_zt = 0;
} linear_regression_calc;

}  // namespace PRISM

#endif