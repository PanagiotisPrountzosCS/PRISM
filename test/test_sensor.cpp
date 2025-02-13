#include <gtest/gtest.h>

#include "core/randomdatamonitor.h"
#include "core/sensor.h"
#include "datagenerators/randomnumberfactory.h"
#include "globaldefinitions.h"

namespace PRISMTESTS {

TEST(Sensor, Construction) {
    PRISM::Sensor s1("sensor1", PRISM::SensorType::PRESSURE, PRISM::DataMonitorType::RANDOM, 50,
                     100, "psi");
    PRISM::Sensor s2("sensor2", PRISM::SensorType::PRESSURE, PRISM::DataMonitorType::RANDOM, 100,
                     200, "psi");

    EXPECT_NE(s1.getId(), s2.getId());
    EXPECT_EQ(s1.getName(), "sensor1");
    EXPECT_EQ(s2.getName(), "sensor2");
}

TEST(Sensor, MeasurementCreation) {
    PRISM::Sensor s("s", PRISM::SensorType::TEMPERATURE, PRISM::DataMonitorType::RANDOM, 50,
                     100, "c");
    auto m = s.createMeasurement(42.0, 1000000);

    EXPECT_EQ(m.value, 1000000);
    EXPECT_EQ(m.timestamp_us, 42.0);
    EXPECT_EQ(m.sensorId, s.getId());

    EXPECT_EQ(s.getLastValue(), 1000000);
    EXPECT_EQ(s.getLastTimestamp(), 42.0);
}

}  // namespace PRISMTESTS