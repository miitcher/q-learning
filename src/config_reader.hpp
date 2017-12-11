#include "agent_learner.hpp"
#include "simulation.hpp"
#include "interactor.hpp"
#include <string>
#include <vector>

/**
Function that reads and parses settings from the given configuration file.
*/
void readAgentLearnerConfigFile(std::string& agentConfigFilename,
    std::vector<Actor>& actors,
    std::vector<Sensor>& sensors,
    AgentShape& agentShape,
    unsigned int& agentCount,
    std::string& qtableFilename,
    bool& drawGraphics);
