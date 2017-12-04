#include "agent_learner.hpp"
#include "interactor.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h> // stat
#include <stdexcept> // std::runtime_error

// Enumerator used by the readAgentLearnerConfigFile.
enum setting {
    Nothing,
    ActorSetting,
    SensorSetting,
    AgentShapeSetting,
    AgentCountSetting,
    QtableFilenameSetting,
    DrawGraphicsSetting
};

std::vector<std::string> commaSplitString(std::string str) {
    std::stringstream ss(str);
    std::vector<std::string> result;
    std::string substr;

    while( ss.good() ) {
        getline( ss, substr, ',' );
        result.push_back( substr ); // std::stod() Makes a substr to a double.
        //std::cout << substr << std::endl;
    }
    return result;
}

void readAgentLearnerConfigFile(std::string& agentConfigFilename,
    std::vector<Actor>& actors,
    std::vector<Sensor>& sensors,
    AgentShape& agentShape,
    unsigned int& agentCount,
    std::string& qtableFilename,
    bool& drawGraphics)
{
    // Check if agentConfigFilename exists.
    struct stat buffer;
    if (stat (agentConfigFilename.c_str(), &buffer) != 0) {
        throw std::runtime_error("File '" + agentConfigFilename
            + "' does not exist.");
    }

    // Default values if they are not found in the config file.
    agentCount = 1;
    qtableFilename = "";
    drawGraphics = true;

    std::string line; // A line in the configuration file.
    std::ifstream configFile(agentConfigFilename);
    if (configFile.is_open()) {
        // currentSetting keeps track witch setting is currently handled.
        setting currentSetting = Nothing;
        unsigned lineNumber = 0; // Debug
        std::vector<std::string> settingParameters; // From csv lines.
        while ( getline(configFile, line) ) {
            lineNumber++;
            // Ignore comments and empty lines
            if (line[0] != '#' && line.size() != 0) {
                //std::cout << line << '\n'; // Debug

                // Handle the currentSetting.
                if (line == "actor") {
                    currentSetting = ActorSetting;
                } else if (line == "sensor") {
                    currentSetting = SensorSetting;
                } else if (line == "agentShape") {
                    currentSetting = AgentShapeSetting;
                } else if (line == "agentCount") {
                    currentSetting = AgentCountSetting;
                } else if (line == "qtableFilename") {
                    currentSetting = QtableFilenameSetting;
                } else if (line == "drawGraphics") {
                    currentSetting = DrawGraphicsSetting;
                } else if ( line[0] == '/') {
                    currentSetting = Nothing;
                } else {

                    // Handle parsing the settings.
                    settingParameters = commaSplitString(line);
                    //std::cout << ' ' << line << '\n'; // Debug
                    switch(currentSetting)
                    {
                        case ActorSetting:
                            if (settingParameters.size() == 5
                                && settingParameters[0] == "JointActor")
                            {
                                JointActor actor(
                                    std::stoi(settingParameters[1]),
                                    std::stoi(settingParameters[2]),
                                    std::stof(settingParameters[3]),
                                    std::stof(settingParameters[4])
                                );
                                actors.push_back(actor);
                            } else {
                                throw std::runtime_error("FAULTY LINE:"
                                    + std::to_string(lineNumber) + ": " + line);
                            }
                            break;
                        case SensorSetting:
                            if (settingParameters.size() == 5
                                && settingParameters[0] == "JointSensor")
                            {
                                JointSensor sensor(
                                    std::stoi(settingParameters[1]),
                                    std::stoi(settingParameters[2]),
                                    std::stof(settingParameters[3]),
                                    std::stof(settingParameters[4])
                                );
                                sensors.push_back(sensor);
                            } else if (settingParameters[0] == "XAxisSensor") {
                                XAxisSensor sensor(
                                    std::stoi(settingParameters[1])
                                );
                                sensors.push_back(sensor);
                            } else {
                                throw std::runtime_error("FAULTY LINE:"
                                    + std::to_string(lineNumber) + ": " + line);
                            }
                            break;
                        case AgentShapeSetting:
                            if (settingParameters.size() == 4
                                && settingParameters[0] == "Body")
                            {
                                // TODO: Change when AgentShape is defined.
                                AgentShape shape = std::stoi(
                                    settingParameters[1]);
                                agentShape = shape;
                            } else {
                                throw std::runtime_error("FAULTY LINE:"
                                    + std::to_string(lineNumber) + ": " + line);
                            }
                            break;
                        case AgentCountSetting:
                            agentCount = std::stoi(line);
                            break;
                        case QtableFilenameSetting:
                            qtableFilename = line;
                            break;
                        case DrawGraphicsSetting:
                            if (line == "true") {
                                drawGraphics = true;
                            } else if (line == "false") {
                                drawGraphics = false;
                            } else {
                                throw std::runtime_error("FAULTY LINE:"
                                    + std::to_string(lineNumber) + ": " + line);
                            }
                            break;
                        case Nothing:
                            throw std::runtime_error("FAULTY LINE:"
                                + std::to_string(lineNumber) + ": " + line);
                            break;
                    } // end of switch
                }
            }
        } // end of while
        configFile.close();
    } else {
        throw std::runtime_error("Unable to open file: " + agentConfigFilename);
    }
}
