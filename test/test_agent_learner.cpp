#include <gtest/gtest.h>
#include "../src/interactor.hpp"
#include "../src/agent_learner.hpp"
#include <iostream>
#include <map>

TEST(test_AgentLearner, test_constructor) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};

    AgentLearner a(actorVec, sensorVec);
    EXPECT_EQ(a.getNumberOfStates(), 13 * 12);
    EXPECT_EQ(a.getNumberOfMoves(), 15 * 33);
    EXPECT_EQ(a.getActors()[0].getID(), a1.getID());
    EXPECT_EQ(a.getSensors()[0].getID(), b.getID());
    EXPECT_EQ(a.getActors()[1].getID(), a2.getID());
    EXPECT_EQ(a.getSensors()[1].getID(), b1.getID());
}

TEST(test_AgentLearner, test_statekeys) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 15, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};

    // test create stateKeys for agent with 2 sensors
    AgentLearner a(actorVec, sensorVec);
    SensorInput si0 = 11;
    SensorInput si1 = 10;
    ResponsePacket rp0(20, si0);
    ResponsePacket rp1(21, si1);
    std::vector<ResponsePacket> rpvec = {rp0, rp1};
    EXPECT_EQ(a.convertStateToKey(rpvec), 101);
    si0 = 199.9999;
    si1 = 199.9999;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpvec = {rp0, rp1};
    EXPECT_EQ(a.convertStateToKey(rpvec), 1513);

    // test create stateKeys for agent with one sensor
    Sensor b2 = Sensor(20, "sensor1", 2, 11, 200);
    actorVec = {a1, a2};
    sensorVec = {b2};
    AgentLearner agent(actorVec, sensorVec);
    si0 = 11;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 1);
    si0 = (200-11)/13;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 1);
    si0 = 2* (200-11)/13;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 2);
    si0 = 3* (200-11)/13;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 3);
    si0 = 199.9999;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 13);
}

TEST(test_AgentLearner, test_actionkeys) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Counterclockwise, Clockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Counterclockwise, Clockwise});
    Sensor b = Sensor(20, "sensor1", 12, 11.111, 200.00);
    Sensor b1 = Sensor(21, "sensor2", 13, 12.3333, 200.1231231);
    std::vector<Actor> actorVec = {a0, a1};
    std::vector<Sensor> sensorVec = {b, b1};
    AgentLearner a(actorVec, sensorVec);

    //for (auto i: a.actionKeys){std::cout << i << std::endl;}

    Action am = a.convertKeyToAction(201);
    EXPECT_EQ(am[0].second, Still);
    EXPECT_EQ(am[1].second, Counterclockwise);

    am = a.convertKeyToAction(303);
    EXPECT_EQ(am[0].second, Clockwise);
    EXPECT_EQ(am[1].second, Clockwise);

    am = a.convertKeyToAction(101);
    EXPECT_EQ(am[0].second, Still);
    EXPECT_EQ(am[1].second, Still);

    am = a.convertKeyToAction(301);
    EXPECT_EQ(am[0].second, Still);
    EXPECT_EQ(am[1].second, Clockwise);

    actorVec = {a0, a1, a2};
    AgentLearner agent(actorVec, sensorVec);

    //std::cout << std::endl;
    //for (auto i: agent.actionKeys){std::cout << i << std::endl;}

    Action am1 = agent.convertKeyToAction(30201);
    EXPECT_EQ(am1[0].second, Still);
    EXPECT_EQ(am1[1].second, Counterclockwise);
    EXPECT_EQ(am1[2].second, Clockwise);

    am1 = agent.convertKeyToAction(20303);
    EXPECT_EQ(am1[0].second, Clockwise);
    EXPECT_EQ(am1[1].second, Clockwise);
    EXPECT_EQ(am1[2].second, Counterclockwise);

    am1 = agent.convertKeyToAction(20101);
    EXPECT_EQ(am1[0].second, Still);
    EXPECT_EQ(am1[1].second, Still);
    EXPECT_EQ(am1[2].second, Counterclockwise);

}

TEST(test_AgentLearner, test_quantizise) {
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
    AgentLearner a(actorVec, sensorVec);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 15), 0);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 20), 0);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 35), 1);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 69), 3);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 120), 6);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 150), 8);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 180), 10);
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 200), -1);

}

TEST(test_AgentLearner, test_public_methods) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 15, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    AgentLearner a(actorVec, sensorVec);
    SensorInput si0 = 11;
    SensorInput si1 = 199.99;
    ResponsePacket rp0(20, si0);
    ResponsePacket rp1(21, si1);
    State state = {rp0, rp1};
    a.receiveSimulationResponse(state);
    EXPECT_EQ(a.getState(), 1501);

    Action act = a.doAction();
    //std::cout << a;
    //std::cout << act[0].first;

}

TEST(test_AgentLearner, test_doAction_and_chooseRandomAction ) {
    // Create agentLearner
    Actor a1 = JointActor(1, 256, 0, 0);
    Actor a2 = JointActor(2, 256, 0, 0);
    Sensor b0 = XAxisSensor(0);
    Sensor b1 = JointSensor(1, 256, 0, 0);
    Sensor b2 = JointSensor(2, 256, 0, 0);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b0, b1, b2};
    AgentLearner agentLearner(actorVec, sensorVec);

    Action action = agentLearner.doAction();
    EXPECT_EQ(action.size(), 2);

    action = agentLearner.chooseRandomAction();
    EXPECT_EQ(action.size(), 2);

    action = agentLearner.chooseRandomAction();
    EXPECT_EQ(action.size(), 2);

    /*
    // Debug
    std::cout << "ACTIONS: " << action[0].second << " "
        << action[1].second << std::endl;
    */
}

/*
// TODO: Mikael
TEST(test_AgentLearner, test_chooseBestAction ) {
    // Create agentLearner
    Actor a1 = JointActor(1, 256, 0, 0);
    Sensor b0 = XAxisSensor(0);
    Sensor b1 = JointSensor(1, 256, 0, 0);
    std::vector<Actor> actorVec = {a1};
    std::vector<Sensor> sensorVec = {b0, b1};
    AgentLearner agentLearner(actorVec, sensorVec);

    EXPECT_EQ(agentLearner.getState(), 0); // Default

    Action action = agentLearner.chooseBestAction();
    EXPECT_EQ(action.size(), 2);

    // Debug
    //std::cout << "ACTIONS: " << action[0].second << " "
    //    << action[1].second << std::endl;
}
*/
