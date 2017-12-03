#include "interactor.hpp"
#include "agent.hpp"
#include "agent_manager.hpp"
#include "config_reader.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

// The main will controll IO and graphics, and start the program.
int main() {
    std::cout << "--- q-learning-9 ---" << std::endl;

    // File that dictates the learning Agent.
    std::string agentConfigFilename = "../test/files/configAgent_test.config";

    // Initialize variables
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;

    // Set variables. This is error prone, because the user can modify the
    // configuration file.
    try {
        readAgentConfigFile(agentConfigFilename, actors, sensors,
            agentShape, agentCount, qtableFilename, drawGraphics);
    } catch (std::runtime_error& e) {
        std::cout << "An error occured, while reading the config file: "
            << e.what() << std::endl;
    }

    // Set AgentManager
    AgentManager agentManager(actors, sensors, agentShape, agentCount,
        qtableFilename, drawGraphics);

    // Start learning
    //agentManager.initRun();
}
