#ifndef QLEARNING_9_AGENT_MANAGER_H_
#define QLEARNING_9_AGENT_MANAGER_H_

#include "agent.hpp"
#include "interactor.hpp"
#include <vector>
#include <string>
#include <thread>

void printHello();

/**
Function for the threads task, that runs the learning and simulation of
one agent.
*/
void agentThreadTask(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, std::string const& qtableFilename);

// TODO: Mikael
// Initializes and controlls the threads where agents and their simulation are.
class AgentManager {
public:
    AgentManager(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, unsigned int agentCount,
        std::string const& qtableFilename, bool drawGraphics)
        : actors(actors), sensors(sensors), agentShape(agentShape),
        agentCount(agentCount), qtableFilename(qtableFilename),
        drawGraphics(drawGraphics) {}

    // Creates threads that contain an agent and its simulation.
    void initRun();

    /**
    Has the fittest agent teach the other agents, when multiple agents
    are learning. The learning is done by copying the Q-values of the
    fittest agent to other agents from the previous generation.
    The Q-learning function parameters (discountFactor, etc.) are also here
    modified.
    */
    void evolveAgents();

    // Pauses all the threads
    void pauseSimulation();

    // Makes the first agent save its Q-values to file
    void saveQvalues();

    // Terminate threads
    void endSimulation();

private:
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    // TODO: Find out how to handle threads
    std::vector<std::thread> agentThreads;
};

// Model-class used before the real Box2D simulation can be used.
// Shows the wanted behaviour.
class Simulation {
public:
    Simulation(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics)
        : actors(actors), sensors(sensors), agentShape(agentShape),
        drawGraphics(drawGraphics) {}

    std::vector<ResponsePacket> simulateAction(
        std::vector<ActionPacket> actionMessage)
    {
        int i = actionMessage[0].first;
        std::pair<int, SensorInput> rp1(i, 3.2);
        std::pair<int, SensorInput> rp2(2, 31.2);
        return {rp1, rp2};
    };
private:
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    bool drawGraphics;
};

/* AgentManager behavior:
Create agent threads, and give them the q-value source (null or filename).
*/

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
