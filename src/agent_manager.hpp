#ifndef QLEARNING_9_AGENT_MANAGER_H_
#define QLEARNING_9_AGENT_MANAGER_H_

#include "agent.hpp"
#include "interactor.hpp"
#include <vector>
#include <string>
#include <thread>

// For access to private methods in AgentManager from googletest.
#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

/**
Task for a threads, where the learning and simulation of one agent is done.
maxLoopCount = 0 makes the task run forever, but otherwise it limits how
long the run will take (used in e.g. testing).
The function is called in AgentManager::initRun().
canSaveQtable states if the Qtable can be saved in a thread. Only one
thread can save its Qtable.
*/
void agentTask(std::vector<Actor> actors, std::vector<Sensor> sensors,
    AgentShape agentShape, std::string qtableFilename,
    bool drawGraphics, unsigned maxLoopCount, bool canSaveQtable);

// TODO: Mikael
// Initializes and controlls the threads where agents and their simulation are.
class AgentManager {
public:
    AgentManager(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, unsigned int agentCount,
        std::string const& qtableFilename, bool drawGraphics);
    /**
    Creates and runs threads that contain an agent and its simulation.
    The threads use agentTask as their task.
    runMode has modes:
        1: Controll from command line.
        2: Controll from GUI (Not implemented)
    */
    void initRun(unsigned runMode);
private:
    // For access to private methods in AgentManager from googletest.
    FRIEND_TEST(test_agentManager, test_initRun_smoketest);

    void createAndStartThreads();
    // Puts all Agent threads on pause.
    void pause_threads();
    // Resumes regular execution of Agent threads, from e.g. a paused state.
    void resume_threads();
    // Waits on the compleation of the Agent tasks.
    // Removes also the joined threads in agentThreads.
    // Because an Agent task generally runs forever, this function
    // should not be called outside the stop_threads(), if it is not
    // known that the tasks will finish.
    void join_threads();
    // Ends cleanly the execution of the Agent tasks.
    // stop_threads() also joins the threads.
    void stop_threads();
    /**
    Makes the first agent save its Qtable to file.
    There is no need to differentiate witch ones Qtable is saved,
    because we do not know witch Agent in one generation is the best.
    */
    void saveQtable();
    /**
    Has the fittest agent teach the other agents, when multiple agents
    are learning. The learning is done by copying the Q-values of the
    fittest agent to other agents from the previous generation.
    The Q-learning function parameters (discountFactor, etc.) are also here
    modified.
    */
    void evolveAgents();

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    std::vector<std::thread> agentThreads;
};

// Dummy/Model-class used before the real Box2D simulation can be used.
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
        // Dummy
        ResponsePacket responsePacket0(actionMessage[0].first, 1.2);
        ResponsePacket responsePacket1(1, 2.6);
        ResponsePacket responsePacket2(2, 5.3);
        return {responsePacket0, responsePacket1, responsePacket2};
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
