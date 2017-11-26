#include <gtest/gtest.h>
#include "../src/learning.hpp"

TEST(test_learning, test_actor) {
    Actor a;
    int v = a.returnTwo();

    EXPECT_EQ(v, 2);
}
