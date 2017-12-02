#include <gtest/gtest.h>
#include "../src/q-table.hpp"
#include <iostream>

TEST(test_Qtable, test_constructor) {
    Qtable t(2,2);
    EXPECT_EQ(t.getNumberOfStates(), 2);
    EXPECT_EQ(t.getNumberOfActions(), 2);
}

TEST(test_Qtable, test_functions) {
    Qtable t(4,4);

    // test updateQvalue
    int x = 0;
    int y = 0;
    QValue z = 0.122;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 0; y = 1; z = 0.1224;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    // test getMaxQvalue and getBestAction
    x = 0; y = 3; z = 0.15;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestAction(x), y);
    x = 1; y = 1; z = 0.15665;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestAction(x), y);
    x = 2; y = 2; z = 0.9155;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestAction(x), y);
    x = 3; y = 0; z = 0.1655;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestAction(x), y);

    // test printQtable
    std::cout << "test printQtable" << std::endl;
    t.printQtable();

    // test getRandomAction
    EXPECT_EQ(true, (t.getRandomAction() < 4) && (t.getRandomAction() >= 0));
}
