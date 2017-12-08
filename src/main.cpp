#include "interactor.hpp"
#include "agent_learner.hpp"
#include "agent_manager.hpp"
#include "config_reader.hpp"
#include "Testbed/Framework/TestbedMain.cpp"
#include "Testbed/Framework/Test.h"
#include "Testbed/Tests/TestCrawler.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// The main will controll IO and graphics, and start the program.
int main() {
    std::cout << "--- q-learning-9 ---" << std::endl;

    // File that dictates the learning AgentLearner.
    std::string agentConfigFilename = "../test/files/configAgent_test.config";

    // Initialize variables
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
	char* argv[] = {"Hello"};
	TestbedMain(1, argv);
    // Set variables. This is error prone, because the user can modify the
    // configuration file.
    try {
        readAgentLearnerConfigFile(agentConfigFilename, actors, sensors,
            agentShape, agentCount, qtableFilename, drawGraphics);
    } catch (std::runtime_error& e) {
        std::cout << "An error occured, while reading the config file: "
            << e.what() << std::endl;
        return -1;
    }

    // Set AgentManager
    AgentManager agentManager(actors, sensors, agentShape, agentCount,
        qtableFilename, drawGraphics);

    // Start learning
    unsigned runMode = 1;
    agentManager.initRun(runMode);

    return 0;
}
