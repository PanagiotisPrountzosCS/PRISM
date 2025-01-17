#include <gtest/gtest.h>

#include "core/objectid.h"

namespace PRISMTESTS {

TEST(ObjectId, DefaultConstructor) {
    PRISM::ObjectId id1;
    PRISM::ObjectId id2;
    EXPECT_NE(id1.getValue(), 0);
    EXPECT_NE(id2.getValue(), 0);
    EXPECT_NE(id1.getValue(), id2.getValue());
    EXPECT_EQ(id1.getValue(), 1);
    EXPECT_EQ(id2.getValue(), 2);
}

TEST(ObjectId, ExplicitConstructor) {
    PRISM::ObjectId id(42);
    EXPECT_EQ(id.getValue(), 42);
    EXPECT_TRUE(id.isValid());
}

TEST(ObjectId, Comparison) {
    PRISM::ObjectId id1;
    PRISM::ObjectId id2;
    PRISM::ObjectId id3(id1.getValue());

    EXPECT_NE(id1, id2);
    EXPECT_EQ(id1, id3);
}

}  // namespace PRISMTESTS