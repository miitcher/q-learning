#include <gtest/gtest.h>
#include "../src/interactor.hpp"
#include "../src/q-table.hpp"
#include "../src/q-table.cpp"
#include "../src/agent.hpp"
#include "../src/agent.cpp"
#include <iostream>

TEST(test_interactor, test_Interactor) {
    Interactor a(42, "generic interactor", 1256, -3.14, 1.1);

    EXPECT_EQ(a.getID(), 42);
    EXPECT_EQ(a.getDescription(), "generic interactor");
    EXPECT_EQ(a.getQuantizationSteps(), 1256);
    EXPECT_EQ(a.getMinAngle(), float(-3.14));
    EXPECT_EQ(a.getMaxAngle(), float(1.1));
}

TEST(test_interactor, test_Actor) {
    Actor a(23, "generic actor", 126, 1, 2, {Still, Clockwise});

    EXPECT_EQ(a.getID(), 23);
    EXPECT_EQ(a.getDescription(), "generic actor");
    EXPECT_EQ(a.getQuantizationSteps(), 126);
    EXPECT_EQ(a.getMinAngle(), float(1));
    EXPECT_EQ(a.getMaxAngle(), float(2));
    EXPECT_EQ(a.getNumberOfActions(), 2);

    auto actions = a.getActions();
    //for (auto i : actions) { std::cout << "Action: " << i << std::endl; }
    EXPECT_EQ(actions[0], 0);
    EXPECT_EQ(actions[1], 2);
}

TEST(test_interactor, test_Sensor) {
    Sensor a(231, "generic sensor", 126, 10, 2);

    EXPECT_EQ(a.getID(), 231);
    EXPECT_EQ(a.getDescription(), "generic sensor");
    EXPECT_EQ(a.getQuantizationSteps(), 126);
    EXPECT_EQ(a.getMinAngle(), float(10));
    EXPECT_EQ(a.getMaxAngle(), float(2));
}

TEST(test_interactor, test_JointActor) {
    JointActor a(2, 456, 0.0, 0);

    EXPECT_EQ(a.getID(), 2);
    EXPECT_EQ(a.getDescription(), "actor of rotating 2D joint");
    EXPECT_EQ(a.getQuantizationSteps(), 456);
    EXPECT_EQ(a.getMinAngle(), float(0));
    EXPECT_EQ(a.getMaxAngle(), float(0));
    EXPECT_EQ(a.getNumberOfActions(), 3);
}

TEST(test_interactor, test_JointSensor) {
    JointSensor a(1, 46, 2, 1);

    EXPECT_EQ(a.getID(), 1);
    EXPECT_EQ(a.getDescription(), "sensor of 2D joints angle");
    EXPECT_EQ(a.getQuantizationSteps(), 46);
    EXPECT_EQ(a.getMinAngle(), float(2));
    EXPECT_EQ(a.getMaxAngle(), float(1));
}

TEST(test_Qtable, test_constructor) {
    Qtable t(2,2);
    EXPECT_EQ(t.getNumberOfStates(), 2);
    EXPECT_EQ(t.getNumberOfActions(), 2);
}

TEST(test_Qtable, test_get_and_update_Qvalue) {
    Qtable t(4,4);

    int x = 1;
    int y = 1;
    QvalueType z = 0.122;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);

    x = 3; y = 3; z = 0.1224;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);
    x = 3; y = 3; z = 0.155;
    t.updateQvalue(x,y,z);
    EXPECT_EQ(t.getQvalue(x,y), z);
}

TEST(test_Agent, test_constructor) {
    /*Actor(int ID, std::string const& description, int quantizationSteps,
    float minAngle, float maxAngle,
        std::vector<ActionType> actions)*/
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});

    /*Interactor(int ID, std::string const& description, int quantizationSteps,
        float minAngle, float maxAngle)*/
    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};

    Agent a(actorVec, sensorVec);
    EXPECT_EQ(a.getNumberOfStates(), 13 * 12);
    EXPECT_EQ(a.getNumberOfActions(), 15 * 33);

}
TEST(test_Agent, test_functions) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Counterclockwise, Clockwise});

    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);
    std::vector<Actor> actorVec = {a0, a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    Agent a(actorVec, sensorVec);
  //  for (auto i : a.actors){std::cout << i.getActions()[1];}
  //  for (auto i : a.sensors){std::cout << i.getID();}
    ActionPacketType ap(21, Counterclockwise);
    ActionPacketType ap1(22, Still);
    ActionPacketType ap2(23, Clockwise);
    std::vector<ActionPacketType> apvec = {ap, ap1, ap2};
    /*
       If there is 3 actors in an agent that have actionVectors:

       a0: {Still, Counterclockwise, Clockwise},
       a1: {Still, Counterclockwise, Clockwise},
       a2: {Counterclockwise, Clockwise},

       or same vectors as enumerations:
       a0: {0, 1, 2},
       a1: {0, 1, 2},
       a2: {1, 2},

       the convertActionToIndex should return an index according to the
       following table:

       index    a2  a1  a0
       0        1   0   0
       1        1   0   1
       2        1   0   2
       3        1   1   0
       4        1   1   1
       5        1   1   2
       6        1   2   0
       7        1   2   1
       8        1   2   2
       9        2   0   0
       10       2   0   1
       11       2   0   2
       12       2   1   0
       13       2   1   1
       14       2   1   2
       15       2   2   0
       16       2   2   1
       17       2   2   2
    */
    EXPECT_EQ(a.convertActionToIndex(apvec), 10);
}
















