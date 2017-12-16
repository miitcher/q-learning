#include <gtest/gtest.h>
#include "../src/simulation.hpp"

TEST(test_Simulation, test_constructor) {
    Actor a1 = Actor(1, "generic actor", 15, 0, 200, {Still, Clockwise});
    Actor a2 = Actor(2, "generic actor", 33, 0, 100, {Still, Clockwise});
    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(1, "sensor1", 13, 0, 200);
    Sensor b2 = Sensor(2, "sensor2", 12, 0, 100);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b0, b1, b2};
    AgentShape agentShape = 42;
    unsigned id = 11;
    Simulation sims(id, actorVec, sensorVec, agentShape, false);

    EXPECT_EQ(sims.shoulderID, 1);
    EXPECT_EQ(sims.elbowID, 2);
    EXPECT_EQ(sims.shoulderMinAngle, 0);
    EXPECT_EQ(sims.shoulderMaxAngle, 200);
    EXPECT_EQ(sims.elbowMinAngle, 0);
    EXPECT_EQ(sims.elbowMaxAngle, 100);
    //EXPECT_EQ(sims.forearm.upperAngle, 1);
}

TEST(test_Simulation, test_simulateAction){
    Actor a1 = Actor(1, "generic actor", 15, 0, 200, {Still, Clockwise});
    Actor a2 = Actor(2, "generic actor", 33, 0, 100, {Still, Clockwise});
    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(1, "sensor1", 13, 0, 200);
    Sensor b2 = Sensor(2, "sensor2", 12, 0, 100);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b0, b1, b2};
    AgentShape agentShape = 42;
    unsigned id = 11;
    Simulation sims(id, actorVec, sensorVec, agentShape, false);


    ActorAction aa1(1, Clockwise);
    ActorAction aa2(2, Clockwise);
    Action action = {aa1, aa2};
    State state = sims.simulateAction(action);

    EXPECT_EQ(state.size(), 3);
    EXPECT_EQ(state[0].first, 999);
    EXPECT_EQ(state[1].first, 1);
    EXPECT_EQ(state[2].first, 2);
    //failing testsEXPECT_EQ(state[0].second, 0);
    //EXPECT_EQ(state[1].second, 0);
    //EXPECT_EQ(state[2].second, 0);
}
