#include <gtest/gtest.h>
#include "../src/interactor.hpp"
#include "../src/q-table.hpp"
#include "../src/agent.hpp"
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

TEST(test_Agent, test_constructor) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    /*(int ID, std::string description, int quantizationSteps,
        float minAngle, float maxAngle)*/
    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};

    Agent a(actorVec, sensorVec);
    EXPECT_EQ(a.getNumberOfStates(), 13 * 12);
    EXPECT_EQ(a.getNumberOfActions(), 15 * 33);
    EXPECT_EQ(a.getActors()[0].getID(), a1.getID());
    EXPECT_EQ(a.getSensors()[0].getID(), b.getID());
    EXPECT_EQ(a.getActors()[1].getID(), a2.getID());
    EXPECT_EQ(a.getSensors()[1].getID(), b1.getID());
}

TEST(test_Agent, test_agents_actor) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Counterclockwise, Clockwise});
    Sensor b = Sensor(20, "sensor1", 12, 11.111, 200.00);
    Sensor b1 = Sensor(21, "sensor2", 13, 12.3333, 200.1231231);
    std::vector<Actor> actorVec = {a0, a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    Agent a(actorVec, sensorVec);
  //  for (auto i : a.actors){std::cout << i.getActions()[1];}
  //  for (auto i : a.sensors){std::cout << i.getID();}
    ActionPacket ap(21, Counterclockwise);
    ActionPacket ap1(22, Still);
    ActionPacket ap2(23, Clockwise);
    std::vector<ActionPacket> apvec = {ap, ap1, ap2};
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

    // test convertActionToIndex
    EXPECT_EQ(a.convertActionToIndex(apvec), 10);
}

TEST(test_Agent, test_quantizise) {
    // Quantizising is done during the learning process so this should not
    // be slow.
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Counterclockwise, Clockwise});
    Sensor b = Sensor(20, "sensor1", 12, 15, 200.00);
    Sensor b1 = Sensor(21, "sensor2", 13, 24.3333, 200.1231231);
    std::vector<Actor> actorVec = {a0, a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    Agent a(actorVec, sensorVec);
    EXPECT_EQ(a.quantiziseSensorInput(b, 5.000), -1);
    EXPECT_EQ(a.quantiziseSensorInput(b, 15.000), 0);
    EXPECT_EQ(a.quantiziseSensorInput(b, 55), 2);
    EXPECT_EQ(a.quantiziseSensorInput(b, 100), 5);
    EXPECT_EQ(a.quantiziseSensorInput(b, 140), 8);
    EXPECT_EQ(a.quantiziseSensorInput(b, 200), 12);

    EXPECT_EQ(a.quantiziseSensorInput(b1, 13.000), -1);
    EXPECT_EQ(a.quantiziseSensorInput(b1, 55), 2);
    EXPECT_EQ(a.quantiziseSensorInput(b1, 100), 5);
    EXPECT_EQ(a.quantiziseSensorInput(b1, 129), 7);
    EXPECT_EQ(a.quantiziseSensorInput(b1, 200), 12);
    EXPECT_EQ(a.quantiziseSensorInput(b1, 2000), -1);
}

TEST(test_Agent, test_agents_sensor) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Counterclockwise, Clockwise});

    Sensor b = Sensor(20, "sensor1", 12, 15, 200.00);
    Sensor b1 = Sensor(21, "sensor2", 13, 24.3333, 200.1231231);
    std::vector<Actor> actorVec = {a0, a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    Agent a(actorVec, sensorVec);

    // test convertResponseToIndex
    SensorInput si0 = 15;
    SensorInput si1 = 30;
    ResponsePacket rp0(20, si0);
    ResponsePacket rp1(21, si1);
/*
    for(int i = 11; i < 21; i++){
        std::cout << a.quantiziseSensorInput
        (b, static_cast<SensorInput>(i)) << " ";
    }
    std::cout << std::endl << std::endl; */

    std::vector<ResponsePacket> rpvec = {rp0, rp1};

    std::cout << "test convertResponseToIndex" << std::endl;
    for(int i = 10; i < 200; i = i+20){
        for(int j = 10; j < 200; j = j+20){
            rp0 = {20, static_cast<SensorInput>(j)};
            rp1 = {21, static_cast<SensorInput>(i)};
            rpvec = {rp0, rp1};
            std::cout << a.convertResponseToIndex(rpvec) << " ";
        }
        std::cout << std::endl;
    }std::cout << std::endl;

    EXPECT_EQ(a.convertResponseToIndex(rpvec), 13*12 -1);
    SensorInput si2 = 11.1;
    SensorInput si3 = 13.1;
    ResponsePacket rp2(20, si2);
    ResponsePacket rp3(21, si3);
    rpvec = {rp2, rp3};
    EXPECT_EQ(a.convertResponseToIndex(rpvec), 0);
}
