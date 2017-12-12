#include <gtest/gtest.h>
#include "../src/agent_manager.hpp"
#include <vector>
#include <string>
#include <fstream>

TEST(test_agentManager, test_agentTask_smoketest) {
    // Smoketest
    Actor a0 = Actor
    (21, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});

    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(1, "sensor1", 13, 11, 200);
    Sensor b2 = JointSensor(2, 12, 10, 200);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1, b2};
    AgentShape agentShape = 42;
    std::string qtableFilename = "qtabletestfile2.bin";
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
    (21, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});
    Actor a1 = Actor
    (22, "generic actor", 15, 1, 200, {Still, Clockwise, Counterclockwise});

    Sensor b0 = XAxisSensor(999);
    Sensor b1 = Sensor(2, "sensor1", 13, 11, 200);
    Sensor b2 = Sensor(1, "sensor2", 12, 10, 200);

    std::vector<Actor> actors = {a0, a1};
    std::vector<Sensor> sensors = {b0, b1, b2};
    AgentShape agentShape = 42;
    unsigned agentCount = 3;
    std::string qtableFilename = "qtabletestfile.bin";
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
