#include <gtest/gtest.h>
#include "../src/agent_manager.hpp"

// TODO: Mikael - These tests should be refined.
/*
TEST(test_agentManager, test_threads1) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a2 = Actor
    (23, "generic actor", 33, 1, 200, {Still, Counterclockwise});

    Sensor b = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);

    std::vector<Actor> actorVec = {a0, a1, a2};
    std::vector<Sensor> sensorVec = {b, b1};

    AgentShape as = 42;

    AgentManager am(actorVec, sensorVec, as, 3, "");

    //am.initRun();
}

TEST(test_agentManager, test_threads2) {
    // Mikael - Code for testing threading and CMake
    printHello();
    std::thread t1(printHello);
    std::cout << "Main thread is executing." << std::endl;
    t1.join();
    std::cout << "Thread joined." << std::endl;

    EXPECT_EQ(1,1);
}
*/
