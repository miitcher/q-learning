#include <gtest/gtest.h>
#include "../src/agent_manager.hpp"
#include <vector>
#include <string>
#include <fstream>

TEST(test_agentManager, test_agentTask_smoketest) {
    // Smoketest
    Actor a0 = Actor
    (1, "generic actor", 15, -1, 0.5, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (2, "generic actor", 15, -0.5, 2.5, {Still, Clockwise, Counterclockwise});

    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(1, "sensor1", 13, -1, 0.5);
    Sensor b2 = JointSensor(2, 12, -0.5, 2.5);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1, b2};
    AgentShape agentShape = 42;
    std::string qtableFilename = "../test/files/fast_qtable.bin";
    bool drawGraphics = false;
    unsigned maxLoopCount = 100;
    bool canSaveQtable = false;

    agentTask(0, actors, sensors, agentShape, qtableFilename,
        drawGraphics, maxLoopCount, canSaveQtable);

}

TEST(test_agentManager, test_initRun_smoketest) {
    // Smoketest
    // The basic running of the program while the treads communicate.
    // The communication is done with the private functions in
    // the AgentManager class.

    Actor a0 = Actor
    (1, "generic actor", 15, -1, 0.5, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (2, "generic actor", 15, -0.5, 2.5, {Still, Clockwise, Counterclockwise});

    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(1, "sensor1", 13, -1, 0.5);
    Sensor b2 = Sensor(2, "sensor2", 12, -0.5, 2.5);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1, b2};
    AgentShape agentShape = 42;
    unsigned agentCount = 3;
    std::string qtableFilename = "../test/files/fast_qtable.bin";
    bool drawGraphics = false;

    AgentManager agentManager(actors, sensors, agentShape, agentCount,
        qtableFilename, drawGraphics);

    agentManager.createAndStartThreads();

    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    agentManager.pause_threads();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    agentManager.resume_threads();
    std::this_thread::sleep_for (std::chrono::milliseconds(700));
    std::string savedFilename = agentManager.saveQtable();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
    agentManager.resume_threads();
    std::this_thread::sleep_for (std::chrono::milliseconds(700));
    agentManager.stop_threads();
    // Check that it does not break from stopping twice.
    agentManager.stop_threads();

    // Remove saved file
    std::remove(savedFilename.c_str());
    EXPECT_TRUE(!std::ifstream(savedFilename));
}
