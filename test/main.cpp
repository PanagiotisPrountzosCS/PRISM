#include <gtest/gtest.h>

#include "app/helpers.h"

double PRISM::appStartTime_ms;

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}