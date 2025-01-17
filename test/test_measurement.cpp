#include <gtest/gtest.h>

#include "core/measurement.h"

namespace PRISMTESTS {

TEST(Measurement, BasicStructure) {
    PRISM::Measurement m;
    m.timestamp_us = 1000000;  // 1 second
    m.readTime_us = 1000100;   // 1.0001 second
    m.value = 42.0;
    m.sensorId = PRISM::ObjectId(1);
    m.errorFlags = 0;

    EXPECT_EQ(m.timestamp_us, 1000000);
    EXPECT_EQ(m.readTime_us, 1000100);
    EXPECT_DOUBLE_EQ(m.value, 42.0);
    EXPECT_EQ(m.sensorId.getValue(), 1);
    EXPECT_EQ(m.errorFlags, 0);
}

TEST(Measurement, Size) { EXPECT_EQ(sizeof(PRISM::Measurement), 40); }

}  // namespace PRISMTESTS