#include <gtest/gtest.h>
#include "../src/q-table.hpp"
#include <iostream>
#include <fstream>

TEST(test_Qtable, test_constructor) {
    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8};
    std::string str = "../test/files/saved_qtable_test.bin";
    Qtable t(stateKeys, actionKeys, str);
    Qtable t1(stateKeys, actionKeys);
    //std::cout << t;
    EXPECT_EQ(t.getStateKeys()[0], 2);
    EXPECT_EQ(t.getActionKeys()[0], 3);
    EXPECT_EQ(t.getQtableFilename(), str);

    ASSERT_THROW(Qtable t({},actionKeys, str), std::invalid_argument);
    ASSERT_THROW(Qtable t(stateKeys, {}, str), std::invalid_argument);
}

TEST(test_Qtable, test_functions) {
    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8};
    std::string str = "../test/files/saved_qtable_test.bin";
    Qtable t(stateKeys, actionKeys, str);
    Qtable t1(stateKeys, actionKeys);

    // test getQvalue
    EXPECT_FLOAT_EQ(t.getQvalue(55, 99), 0.1);
    ASSERT_THROW(t.getQvalue(-1, 99), std::out_of_range);
    ASSERT_THROW(t.getQvalue(55, 0), std::out_of_range);


    // test updateQvalue
    QValue z = 0.122;
    ASSERT_THROW(t.updateQvalue(55, 0, z), std::out_of_range);
    ASSERT_THROW(t.updateQvalue(-999, 99, z), std::out_of_range);

    int x = 7;
    int y = 8;
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

    std::vector<int> stateKeys = {2,55,66,7,2};
    std::vector<int> actionKeys = {3,88,99,8,2};
    std::string str = "qtabletestfile3.bin";
    Qtable t(stateKeys, actionKeys);

    int state = 7;
    int action = 8;
    QValue value = 0.122;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    state = 7; action = 99; value = 0.164;
    t.updateQvalue(state,action,value);
    EXPECT_EQ(t.getQvalue(state,action), value);

    state = 7; action = 3; value = 0.122;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    state = 7; action = 2; value = 0.164;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    state = 55; action = 99; value = 0.164;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    state = 66; action = 3; value = 0.122;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    state = 2; action = 2; value = 0.164;
    t.updateQvalue(state,action,value);
    EXPECT_FLOAT_EQ(t.getQvalue(state,action), value);

    //std::cout << t;

    // Save this q table to file
    t.saveToFile(str);

    // New table is created, every value is 0.1
    Qtable tableCopy(stateKeys, actionKeys);

    state = 7; action = 3;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), 0.1);

    state = 7; action = 2;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), 0.1);

    state = 55; action = 99;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), 0.1);

    state = 66; action = 3;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), 0.1);

    state = 2; action = 2;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), 0.1);

    //std::cout << tableCopy;
    tableCopy.loadFromFile(str);
    //std::cout << tableCopy;

    // Qtable is loaded from file:
    state = 7; action = 3; value = 0.122;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), value);

    state = 7; action = 2; value = 0.164;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), value);

    state = 55; action = 99; value = 0.164;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), value);

    state = 66; action = 3; value = 0.122;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), value);

    state = 2; action = 2; value = 0.164;
    EXPECT_FLOAT_EQ(tableCopy.getQvalue(state,action), value);

    // Remove saved file
    std::remove(str.c_str());
    EXPECT_TRUE(!std::ifstream(str));
}
