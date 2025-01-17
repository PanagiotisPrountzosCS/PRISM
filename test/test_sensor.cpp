#include <gtest/gtest.h>

#include "globaldefinitions.h"
#include "core/sensor.h"

namespace PRISMTESTS {

TEST(Sensor, Construction) {
    PRISM::Sensor s1("sensor1", PRISM::SensorType::TEMPERATURE);
    PRISM::Sensor s2("sensor2", PRISM::SensorType::PRESSURE);

    EXPECT_NE(s1.getId(), s2.getId());
    EXPECT_EQ(s1.getName(), "sensor1");
    EXPECT_EQ(s2.getName(), "sensor2");
}

TEST(Sensor, MeasurementCreation) {
    PRISM::Sensor s("test", PRISM::SensorType::TEMPERATURE);
    auto m = s.createMeasurement(42.0, 1000000);

    EXPECT_EQ(m.value, 42.0);
    EXPECT_EQ(m.timestamp_us, 1000000);
    EXPECT_EQ(m.sensorId, s.getId());
    EXPECT_EQ(m.errorFlags, 0);

    EXPECT_EQ(s.getLastValue(), 42.0);
    EXPECT_EQ(s.getLastTimestamp(), 1000000);
}

}  // namespace PRISMTESTS