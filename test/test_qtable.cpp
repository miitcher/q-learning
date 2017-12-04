#include <gtest/gtest.h>
#include "../src/q-table.hpp"
#include <iostream>

TEST(test_Qtable, test_constructor) {
    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8};
    std::string str = "asd";
    Qtable t(stateKeys, actionKeys, str);
    Qtable t1(stateKeys, actionKeys);
    //std::cout << t;
    EXPECT_EQ(t.getStateKeys()[0], 2);
    EXPECT_EQ(t.getActionKeys()[0], 3);
    EXPECT_EQ(t.getQtableFilename(), "asd");
    EXPECT_EQ(t1.getQtableFilename(), "");
}

TEST(test_Qtable, test_functions) {
    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8};
    std::string str = "asd";
    Qtable t(stateKeys, actionKeys, str);
    Qtable t1(stateKeys, actionKeys);

    // test getQvalue
    EXPECT_EQ(t.getQvalue(55, 99), 0);


  // test updateQvalue
    int x = 7;
    int y = 8;
    QValue z = 0.122;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 55; y = 99; z = 0.164;
    EXPECT_EQ(t.updateQvalue(x,y,z), true);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 0; y = 3;
    EXPECT_EQ(t.updateQvalue(x,y,z), false);


    // test getMaxQvalue
    EXPECT_FLOAT_EQ(t.getMaxQvalue(7), 0.122);
    EXPECT_FLOAT_EQ(t.getMaxQvalue(55), 0.164);
    EXPECT_FLOAT_EQ(t.getMaxQvalue(2), 0);

    // TEST getBestAction
    EXPECT_EQ(t.getBestAction(7), 8);
    EXPECT_EQ(t.getBestAction(55), 99);
    // when equal, returns the first
    EXPECT_EQ(t.getBestAction(2), 3);
}
