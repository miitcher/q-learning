#ifndef QLEARNING_9_LEARNING_H_
#define QLEARNING_9_LEARNING_H_

#include <vector>
#include <string>
#include <thread>

// TODO: Mikael
// Initializes and controlls the threads where agents and their simulation are.
class AgentManager {
public:
    AgentManager(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShapeType& agentShape, unsigned int agentCount);

    // Function used by threads to run the learning and simulation of one agent
    void agentThreadTask();
    // TODO: add parameter for agent and simulation
    // classes, and possibly saved Q-values

    // Reads files and creates threads that contain an agent and its simulation
    void initRun(std::string simulationFileName, std::string qvaluesFilename);

    // Has the fittest agent teach the other agents, when multiple agents
    // are learning. Copies the Q-values of the fittest agent to other agents
    // from the previous eneration and modifies the parameters in the
    // Q-learning function (discountFactor, etc.).
    void evolveAgents();

    // Pauses all the threads
    void pauseSimulation();

    // Makes the first agent save its Q-values to file
    void saveQvalues();

    // Terminate threads
    void endSimulation();

private:
    // TODO: Find out how to handle threads
    std::vector<std::thread> agentThreads;
};

/* TODO: Mikael - THOUGHTS ABOUT THREADING (connected to AgentManager threads)
Use mutex on q-table when copying it between generations.
The mutex locks the resours. The mutex would be in the copy function of the
q-table.

The copying of a q-table from one thread to many threads could be done
by giving the other threads a reference or pointer to the q-table

When a agent gets to the goal, all agents will stop moving and then some
signal/event happens
that tells all the agents to move to the beginning (change state in
learning and in simulation) and copying the q-table.


Alternatively all but the winer would survive between generations, and
the other new Agents for the next generation would be created from scratch.
Thus the new Agents Q-table would be made by the copy constructor.

Could always create one Agent, and then copy it. This would be done
in the beginning of the run and between every genereation.
With only one Agent the only difference would be not copying and
that there is no goal.

condition_variable could be used to have the thread communicate with each
other.


Is the method of discarding all but the winners Q-values effective
computatively?
*/

#endif
