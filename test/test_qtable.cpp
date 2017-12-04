#include <gtest/gtest.h>
#include "../src/q-table.hpp"
#include <iostream>

TEST(test_Qtable, test_constructor) {
    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8};
    std::string str = "asd";
    Qtable t(stateKeys, actionKeys, str);
    Qtable t1(stateKeys, actionKeys);
    // std::cout << t;
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
    int sKey = 2;
    t.getQvalue(2,3);

/*
  // test updateQvalue
    int x = 0;
    int y = 0;
    QValue z = 0.122;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 0; y = 1; z = 0.1224;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    // test getMaxQvalue and getBestMove
    x = 0; y = 3; z = 0.15;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestMove(x), y);
    x = 1; y = 1; z = 0.15665;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestMove(x), y);
    x = 2; y = 2; z = 0.9155;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestMove(x), y);
    x = 3; y = 0; z = 0.1655;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getMaxQvalue(x), z);
    EXPECT_EQ(t.getBestMove(x), y);

    // test printQtable
    // std::cout << "test printQtable" << std::endl;
    // t.printQtable();

    // test getRandomMove
    EXPECT_EQ(true, (t.getRandomMove() < 4) && (t.getRandomMove() >= 0));
 */
}

