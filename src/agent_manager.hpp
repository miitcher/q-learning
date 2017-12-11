#ifndef QLEARNING_9_AGENT_MANAGER_H_
#define QLEARNING_9_AGENT_MANAGER_H_

#include "simulation.hpp"
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
private:
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    std::vector<std::thread> agentThreads;
};

#endif
