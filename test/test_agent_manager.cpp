#include <gtest/gtest.h>
#include "../src/agent_manager.hpp"
#include <vector>
#include <string>

TEST(test_agentManager, test_agentTask) {
    // Smoketest
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});

    Sensor b0 = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1};
    AgentShape agentShape = 42;
    std::string qtableFilename = "";
    bool drawGraphics = false;
    unsigned maxLoopCount = 100;

    agentTask(actors, sensors, agentShape, qtableFilename,
        drawGraphics, maxLoopCount);
}

/*
TEST(test_agentManager, test_initRun) {
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});

    Sensor b0 = Sensor(20, "sensor1", 13, 11, 200);
    Sensor b1 = Sensor(21, "sensor2", 12, 10, 200);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1};
    AgentShape agentShape = 42;
    unsigned agentCount = 3;
    std::string qtableFilename = "";
    bool drawGraphics = false;

    AgentManager agentManager(actors, sensors, agentShape, agentCount,
        qtableFilename, drawGraphics);

    agentManager.initRun();
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
