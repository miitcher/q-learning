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
    EXPECT_FLOAT_EQ(t.getQvalue(55, 99), 0.1);


  // test updateQvalue
    int x = 7;
    int y = 8;
    QValue z = 0.122;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 55; y = 99; z = 0.164;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    // test getMaxQvalue
    EXPECT_FLOAT_EQ(t.getMaxQvalue(7), 0.122);
    EXPECT_FLOAT_EQ(t.getMaxQvalue(55), 0.164);
    EXPECT_FLOAT_EQ(t.getMaxQvalue(2), 0.1);

    // TEST getBestAction
    EXPECT_EQ(t.getBestAction(7), 8);
    EXPECT_EQ(t.getBestAction(55), 99);
    // when equal, returns the first
    EXPECT_EQ(t.getBestAction(2), 3);
}

TEST(test_Qtable, test_save_and_load) {
    // These methods are test by just printing the q tables out.
    // Uncomment prints to test.

    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8,2};
    std::string str = "testfile3.bin";
    Qtable t(stateKeys, actionKeys, str);

    int state = 7;
    int action = 8;
    QValue value = 0.122;
    t.updateQvalue(state,action,value);

    state = 7; action = 99; value = 0.164;
    t.updateQvalue(state,action,value);

    state = 7; action = 3; value = 0.122;
    t.updateQvalue(state,action,value);

    state = 7; action = 2; value = 0.164;
    t.updateQvalue(state,action,value);

    state = 55; action = 99; value = 0.164;
    t.updateQvalue(state,action,value);

    state = 66; action = 3; value = 0.122;
    t.updateQvalue(state,action,value);

    state = 2; action = 2; value = 0.164;
    t.updateQvalue(state,action,value);

    //std::cout << t;
    t.saveToFile();

    stateKeys = {2,55,66,7,2};
    actionKeys = {3,88,99,8,2};
    str = "testfile3.bin";
    Qtable tableCopy(stateKeys, actionKeys, str);

    //std::cout << tableCopy;

    tableCopy.loadFromFile();

    //std::cout << tableCopy;
}
