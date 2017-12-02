#include "agent.hpp"
#include <string>
#include <vector>

/**
Function that reads and parses settings from the given configuration file.
*/
void readAgentConfigFile(std::string& agentConfigFilename,
    std::vector<Actor>& actors,
    std::vector<Sensor>& sensors,
    AgentShape& agentShape,
    unsigned int& agentCount,
    std::string& qtableFilename,
    bool& drawGraphics);
