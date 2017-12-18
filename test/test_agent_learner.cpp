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

    ASSERT_THROW(AgentLearner a ({}, sensorVec), std::invalid_argument);
    ASSERT_THROW(AgentLearner a (actorVec, {}), std::invalid_argument);

    AgentLearner a(actorVec, sensorVec);
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
    //std::cout << a;

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
    actorVec = {a1, a2};
    sensorVec = {b1};
    AgentLearner agent(actorVec, sensorVec);

    si0 = 122;
    rp0 = std::make_pair(20, si0);
    rpvec = {rp0};
    EXPECT_EQ(a.convertStateToKey(rpvec), 8);

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

    //this throws exception because maxangle is exclusive
    // limit "input < maxAngle = 200"
    //ASSERT_THROW(a.quantiziseSensorInput(b.getID(), 200), std::out_of_range);

    //this is ok because minAngle is inclusive limit "input >= minAngle = 15"
    EXPECT_EQ(a.quantiziseSensorInput(b.getID(), 15), 0);
}

TEST(test_AgentLearner, test_chooseBestAction) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    Sensor b = Sensor(20, "sensor1", 4, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 5, 10, 200);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    AgentLearner a(actorVec, sensorVec);

    QValue v = 0.22;
    a.qtable.updateQvalue(501, 301, v);
    v = 0.3;
    a.qtable.updateQvalue(501, 101, v);
    v = 0.66;
    a.qtable.updateQvalue(501, 303, v);
    v = 0.11;
    a.qtable.updateQvalue(501, 103, v);

    v = 0.22;
    a.qtable.updateQvalue(401, 301, v);
    v = 0.999;
    a.qtable.updateQvalue(401, 101, v);
    v = 0.2;
    a.qtable.updateQvalue(401, 303, v);
    v = 0.11;
    a.qtable.updateQvalue(401, 103, v);

    a.currentStateKey = 501;
    Action action = a.chooseBestAction();   // should be 303
    EXPECT_EQ(action[0].second, Clockwise);
    EXPECT_EQ(action[1].second, Clockwise);

    a.currentStateKey = 401;
    action = a.chooseBestAction();          // should be 101
    EXPECT_EQ(action[0].second, Still);
    EXPECT_EQ(action[1].second, Still);
}


TEST(test_AgentLearner, test_receive_simulation_response) {
    Actor a1 = Actor(22, "generic actor", 15, 1, 200, {Still, Clockwise});
    Actor a2 = Actor(23, "generic actor", 33, 1, 200, {Still, Clockwise});
    Sensor b = Sensor(20, "sensor1", 4, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 5, 10, 200);
    Sensor xaxis = XAxisSensor(999);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};
    AgentLearner a(actorVec, sensorVec);

    //std::cout << a;
    //std::cout << a.qtable;

    // In the real run of the program, the simulation determines these values.
    a.location = 0;
    a.previousLocation = 0;

    SensorInput si0 = 11;
    SensorInput si1 = 199.99;
    SensorInput x = 10;
    ResponsePacket rp0(20, si0);
    ResponsePacket rp1(21, si1);
    ResponsePacket rpxaxis(999, x);
    State state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    EXPECT_EQ(a.currentStateKey, 501);
    EXPECT_EQ(a.previousStateKey, 101);
    EXPECT_EQ(a.location, 10);
    EXPECT_EQ(a.previousLocation, 0);

    si0 = 115;
    si1 = 100.99;
    x = 8;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpxaxis = std::make_pair(999, x);
    state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    EXPECT_EQ(a.currentStateKey, 303);
    EXPECT_EQ(a.previousStateKey, 501);
    EXPECT_EQ(a.location, 8);
    EXPECT_EQ(a.previousLocation, 10);

    si0 = 115;
    si1 = 100.99;
    x = 5;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpxaxis = std::make_pair(999, x);
    state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    si0 = 115;
    si1 = 100.99;
    x = 2;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpxaxis = std::make_pair(999, x);
    state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    si0 = 115;
    si1 = 100.99;
    x = 0;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpxaxis = std::make_pair(999, x);
    state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    si0 = 115;
    si1 = 100.99;
    x = -10;
    rp0 = std::make_pair(20, si0);
    rp1 = std::make_pair(21, si1);
    rpxaxis = std::make_pair(999, x);
    state = {rp0, rp1, rpxaxis};
    a.receiveSimulationResponse(state);

    //std::cout << a.qtable;

}

TEST(test_AgentLearner, test_doAction_and_chooseRandomAction ) {
    // Create agentLearner
    Actor a1 = JointActor(1, 256, 0, 0);
    Actor a2 = JointActor(2, 256, 0, 0);
    //Sensor b0 = XAxisSensor(0);
    Sensor b1 = JointSensor(1, 256, 0, 0);
    Sensor b2 = JointSensor(2, 256, 0, 0);
    std::vector<Actor> actorVec = {a1, a2};
    std::vector<Sensor> sensorVec = {b1, b2};
    AgentLearner agentLearner(actorVec, sensorVec);

    Action action = agentLearner.doAction();
    EXPECT_EQ(action.size(), 2);

    action = agentLearner.chooseRandomAction();
    EXPECT_EQ(action.size(), 2);

    action = agentLearner.chooseRandomAction();
    EXPECT_EQ(action.size(), 2);
}
