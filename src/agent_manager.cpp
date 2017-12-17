#include "agent_manager.hpp"
#include "agent_learner.hpp"
#include "interactor.hpp"
#include "simulation.hpp"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm> // for_each
#include <mutex>
#include <utility>
#include <math.h> // pow
#include <atomic>
#include <system_error>
#include <fstream>

// The filename for the fittest saved Q-table, that is used for the evolution
// of the Agents, when multiple Agents are learning.
std::string evolutionFittestQtableFilename
    = "generations_fittest_qtable_in_evolution.bin";

// Controlls the use of the "evolutionFittestQtableFilename" file.
std::mutex evolutionFittestFile_mutex;

// Controlls the use of cout between threads.
std::mutex cout_mutex;

// String for keeping track of what the saved filename is in a thread.
// Protected by mutex.
std::string savedQtableFilename("");

// Mutex protecting savedQtableFilename.
std::mutex savedQtableFilename_mutex;

// Counter that will keep track of how many Agents are waiting on to
// update their Q-table with the fittest Q-table.
// 0 means that no agents are waiting.
std::atomic_uint agentsWaitingToEvolve_counter(0);

/**
Enable log and debug print with threads.
*/
std::atomic_bool useLogging(false);
std::atomic_bool useDebugging(false);

/**
Atomics are thread safe, and they are used here to communicate
between threads.
*/
std::atomic_bool endThreads(false);
std::atomic_bool pauseThreads(false);
/**
When saveQtableInThread is true, then one thread will save its Qtable to file.
A threads Qtable can be saved if canSaveQtable is true.
*/
std::atomic_bool saveQtableInThread(false);
// If only on Agent is used, then no goal is used, but otherwise it is used.
std::atomic_bool useEvolution(true);
std::atomic_bool anAgentHasReachedTheGoal(false);

void agentTask(unsigned agentID,
    std::vector<Actor> actors, std::vector<Sensor> sensors,
    AgentShape agentShape, std::string qtableFilename,
    bool drawGraphics, unsigned maxLoopCount, bool canSaveQtable)
{
    //std::cout << "agentID: " << agentID << std::endl;

    //std::thread::id thisThreadId(std::this_thread::get_id());

    // TODO (if time): Take the goal location as an input parameter.
    // This is maby in the quantizised units???
    SensorInput agentXAxisLocationOfGoal = 3000;

    // Create AgeneLearner and Simulation that makes the Agent.
    AgentLearner agentLearner(actors, sensors, qtableFilename);
    Simulation simulation(agentID, actors, sensors, agentShape, drawGraphics);

    // Have the Simulation:s and AgentLearner:s state at the beginning.
    State state = simulation.moveAgentToBeginning();
    agentLearner.receiveStartingState(state);

    // Initiate variable.
    Action action;

    // count used for debugging and testing.
    unsigned count = 0;
    while (true) {
        // The learning and simulation parts communicate.
        try {
            action = agentLearner.doAction();
            state = simulation.simulateAction(action);
            agentLearner.receiveSimulationResponse(state);

        } catch (std::exception& e) {
            endThreads = true;
            std::cerr << "exeption when learning functions called from agentTask: "
                        << e.what() << '\n';
        }

        try {

            // EVOLUTION RELATED CODE START!

            /*
            - About the evolution of the Agents, when multiple Agents are learning:

            The fittest AgentLearner saves its Qtable to the filename:
                evolutionFittestQtableFilename
            This filename is reserved for the evulution.
            An error is thrown if the mentioned file already exists.

            The fittest Qtable file is then read by all the other AgentLearners,
            thus updating all the AgentLearners Q-tables to the fittest Q-table.
            The reading of the file is controlled with the mutex:
                evolutionFittestFile_mutex
            When the Q-tables have been set, the Agents bodyes will also move to the
            starting position.
            */

            /*
            Evolution fittest Agent.
            If an Agent has reached the goal its the fittest of the Agents
            that are running, and will "teach" the other Agents. The "teaching"
            is done by having the other Agents copy the fittest Agents Qtable into
            their Qtable. The modified Agents form the next generation.
            */
            if (// There is more than one Agent learning.
                useEvolution
                // No other Agent has reached the goal.
                && !anAgentHasReachedTheGoal
                // This Agent has reached the goal.
                && agentLearner.getXAxisLocation() > agentXAxisLocationOfGoal)
            {
                // This thread contains the fittest Agent this generation.
                anAgentHasReachedTheGoal = true;

                // Save the fittest Q-table to file, so the other Agents can copy it.
                evolutionFittestFile_mutex.lock();
                // Remove the reserved file if it exist. It should not.
                if (std::ifstream(evolutionFittestQtableFilename)) {
                    if (useDebugging) {
                        cout_mutex.lock();
                        std::cout << agentID
                            << " Evolution fittest: Remove existing qtable"
                            << std::endl;
                        cout_mutex.unlock();
                    }
                    std::remove(evolutionFittestQtableFilename.c_str());
                    if (std::ifstream(evolutionFittestQtableFilename)) {
                        // End execution and throw error, when the file is not deleted.
                        endThreads = true;
                        throw std::runtime_error("File '"
                            + evolutionFittestQtableFilename + "' not removed.");
                    }
                }
                if (useDebugging) {
                    cout_mutex.lock();
                    std::cout << agentID << " Evolution fittest: Save qtable"
                        << std::endl;
                    cout_mutex.unlock();
                }
                agentLearner.saveQtable(evolutionFittestQtableFilename);
                evolutionFittestFile_mutex.unlock();

                // Wait untill the other Agents have copied the fittest Q-table.
                while (agentsWaitingToEvolve_counter != 0) {
                    std::this_thread::sleep_for (std::chrono::milliseconds(10));
                }

                // Remove the fittest Q-table file.
                evolutionFittestFile_mutex.lock();
                if (useDebugging) {
                    cout_mutex.lock();
                    std::cout << agentID << " Evolution fittest: Remove qtable"
                        << std::endl;
                    cout_mutex.unlock();
                }
                std::remove(evolutionFittestQtableFilename.c_str());
                if (std::ifstream(evolutionFittestQtableFilename)) {
                    // End execution and throw error, when the file is not deleted.
                    endThreads = true;
                    throw std::runtime_error("File '"
                        + evolutionFittestQtableFilename + "' not removed.");
                }
                evolutionFittestFile_mutex.unlock();

                // Debug   TODO: remove, when "resetting" works
                std::cout << agentID << " Evolution fittest: before X="
                    << agentLearner.getXAxisLocation() << std::endl;

                // Have the Simulation:s and AgentLearner:s state at the beginning.
                state = simulation.moveAgentToBeginning();
                agentLearner.receiveStartingState(state);

                // Debug   TODO: remove, when "resetting" works
                std::cout << agentID << " Evolution fittest: after  X="
                    << agentLearner.getXAxisLocation() << std::endl;

                // Resume the normal running of the learning.
                anAgentHasReachedTheGoal = false;
                pauseThreads = false;
            }

            /*
            Evolution Agents other than the fittest Agent.
            When the fittest Agent has reached the goal, the other agent will
            wait until they can read the saved Q-table.
            */
            if (anAgentHasReachedTheGoal)
            {
                // Add this Agent to the Agents that are waiting on their turn to
                // update its Q-table, by incrementing this counter:
                agentsWaitingToEvolve_counter++;

                // Let the fittest Agent take controll of evolutionFittestFile_mutex.
                // TODO: MABY NOT NEEDED...
                std::this_thread::sleep_for (std::chrono::milliseconds(100));

                // Set the Q-table for all the Agents to the fittest agents Q-table.
                evolutionFittestFile_mutex.lock();
                if (useDebugging) {
                    cout_mutex.lock();
                    std::cout << agentID << " Evolution other: Load qtable"
                        << std::endl;
                    cout_mutex.unlock();
                }
                agentLearner.loadQtable(evolutionFittestQtableFilename);
                evolutionFittestFile_mutex.unlock();

                // Have the Simulation:s and AgentLearner:s state at the beginning.
                state = simulation.moveAgentToBeginning();
                agentLearner.receiveStartingState(state);

                // This thread, and the threads not containing the fittest Agent,
                // will now pause until all the Agents are done updating
                // their Q-tables. The paused thread is executing in the loop
                // here under, in the while(pauseThreads)-loop.
                pauseThreads = true;

                // Remove this Agent from the Agents that are waiting on their turn
                // to update their Q-table, by decreasing this counter:
                agentsWaitingToEvolve_counter--;
            }

            // EVOLUTION RELATED CODE END!

        } catch (std::exception& e) {
            endThreads = true;
            std::cerr << "exeption in Agent evolution: " << e.what() << '\n';

        }

        count++;
        if (maxLoopCount != 0 && count > maxLoopCount)
            break;

        if (useLogging) {
            int loop_count_between_output = pow(10, 3) * 5;
            if (count % loop_count_between_output == 0) {
                cout_mutex.lock();
                //std::cout << count / loop_count_between_output << std::endl;
                std::cout << int(count / 1000) << "k: Agent_" << agentID
                    << " X=" << agentLearner.getXAxisLocation() << std::endl;
                cout_mutex.unlock();
            }
        }

        /*
        Listen to commands from the main thread.
        Actions that can be requested:
            pause self; resume self; end self; save Qtable
        */
        while (pauseThreads) {
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
            if (canSaveQtable && saveQtableInThread) {
                savedQtableFilename_mutex.lock();
                savedQtableFilename = agentLearner.saveQtable();
                savedQtableFilename_mutex.unlock();
                saveQtableInThread = false;
            }
        }
        if (endThreads)
            break;
    }
}

AgentManager::AgentManager(std::vector<Actor>& actors,
    std::vector<Sensor>& sensors, AgentShape& agentShape,
    unsigned int agentCount, std::string const& qtableFilename,
    bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    agentCount(agentCount), qtableFilename(qtableFilename),
    drawGraphics(drawGraphics) {}

void AgentManager::pause_threads() {
    pauseThreads = true;
    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Paused" << std::endl;
        cout_mutex.unlock();
    }
}

void AgentManager::resume_threads() {
    pauseThreads = false;
    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Resumed" << std::endl;
        cout_mutex.unlock();
    }
}

// Helper function for AgentManager::join_threads().
void do_join(std::thread& t) { t.join(); }

void AgentManager::join_threads() {
    try {
        std::for_each(agentThreads.begin(), agentThreads.end(), do_join);

        // Remove all the joined threads.
        agentThreads.clear();
    } catch (std::system_error& e) {
        cout_mutex.lock();
        std::cout << "Error during joining of threads: " << e.what()
            << std::endl;
        cout_mutex.unlock();
    }
}

void AgentManager::stop_threads() {
    pauseThreads = false;
    endThreads = true;
    join_threads();
    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Stopped" << std::endl;
        cout_mutex.unlock();
    }
}

void AgentManager::createAndStartThreads() {
    unsigned maxLoopCount = 0;
    bool canSaveQtable;

    // Create threads in vector agentThreads.
    for ( unsigned agentID = 0; agentID != agentCount; agentID++) {
        // Only one thread can have its Qtable saved; the first one.
        if (agentID == 0) {
            canSaveQtable = true;
        } else {
            canSaveQtable = false;
        }
        agentThreads.emplace_back(agentTask, agentID, actors, sensors,
            agentShape, qtableFilename, drawGraphics,
            maxLoopCount, canSaveQtable);
    }

    // Evolution is used with generating new generations of Agents
    // if there is more than one Agent thread.
    if (agentCount == 1)
        useEvolution = false;
}

void AgentManager::initRun() {
    // Print more information to cout.
    useLogging = true;
    //useDebugging = true;

    createAndStartThreads();

    // Give instructions how the program is used from the command line
    cout_mutex.lock();
    std::cout   << " " << agentCount << " Agents learning\n"
                << "--------------------------------------\n"
                << "Do things by typing a command and pressing enter.\n"
                << "Unrecognized commands will be ignored.\n"
                << "p: pause; s: save Qtable; r: resume; q: stop\n"
                << "---------------------------------------------"
                << std::endl;
    cout_mutex.unlock();

    // Take user input from commandline.
    std::string userInput;
    char command;
    while (command != 'q') {
        // If an exeption is thrown in an Agent thread, then all threads
        // has to be terminated, when the program exits cleanly.
        if (endThreads) {
            stop_threads();
            break;
        }

        // The first character is only considered as the command from the
        // users input.
        std::cin >> userInput;
        command = userInput[0];
        switch (command)
        {
            case 'p':
                pause_threads();
                break;
            case 's':
                saveQtable();
                break;
            case 'r':
                resume_threads();
                break;
            case 'q':
                stop_threads();
                break;
        }
    }
}

std::string AgentManager::saveQtable() {
    // Saving is done when all the threads are paused.
    pause_threads();

    // Check that threre is some Agent threads.
    if (agentThreads.empty())
        throw std::runtime_error("There exists no Agent threads.");

    // Save the first agents Qtable.
    saveQtableInThread = true;

    // Wait untill the Qtable has been saved.
    while (saveQtableInThread) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Paused" << std::endl;
        cout_mutex.unlock();
    }

    savedQtableFilename_mutex.lock();
    std::string savedFilename = savedQtableFilename;
    savedQtableFilename_mutex.unlock();
    return savedFilename;
}
