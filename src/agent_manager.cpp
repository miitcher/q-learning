#include "agent_manager.hpp"
#include "agent.hpp"
#include "interactor.hpp"
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

// Controlls the use of cout between threads.
std::mutex cout_mutex;

/**
Enable logging with treads. There is more information on the screen
when this is true.
*/
std::atomic_bool useLogging(false);

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
std::atomic_bool useEvolutionGoal(true);
std::atomic_bool agentHasReachedGoal(false);

void agentTask(std::vector<Actor> actors, std::vector<Sensor> sensors,
    AgentShape agentShape, std::string qtableFilename,
    bool drawGraphics, unsigned maxLoopCount, bool canSaveQtable)
{
    //std::thread::id thisThreadId(std::this_thread::get_id());

    float agentXAxisLocationOfGoal = 30; // Simulation units ???

    Agent agent(actors, sensors, qtableFilename);
    Simulation simulation(actors, sensors, agentShape, drawGraphics);

    unsigned count = 0;
    while (true) {
        // The learning and simulation parts communicate.
        ActionMessage actionMessage = agent.doAction();
        ResponseMessage responseMessage = simulation.simulateAction(
            actionMessage);
        agent.receiveSimulationResponse(responseMessage);

        /* If an Agent has reached the goal its the fittest of the Agents
        that are running, and will "teach" the other Agents. The "teaching"
        is done by having the other Agents copy the fittest Agents
        Qtable. The modified Agents form the next generation.
        */
        if (useEvolutionGoal && !agentHasReachedGoal
            && agent.getXAxisLocation() > agentXAxisLocationOfGoal)
        {
            agentHasReachedGoal = true;
            // Now we know that this thread contains the fittest Agent.
            // TODO: Implement the copying of the Qtable and all that
            // is connected to the evolution of a generation.
        }

        count++;
        if (maxLoopCount != 0 && count > maxLoopCount)
            break;

        /*
        // Debug
        if (count % int(pow(10, 3)) == 0) {
            cout_mutex.lock();
            std::cout << count / int(pow(10, 3)) << std::endl;
            cout_mutex.unlock();
        }
        cout_mutex.lock();
        std::cout << count << std::endl;
        cout_mutex.unlock();
        */

        /*
        Listen to commands from the main thread.
        Actions that can be requested:
            pause self; resume self; end self; save Qtable
        */
        while (pauseThreads) {
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
            if (canSaveQtable && saveQtableInThread) {
                agent.saveQtable();
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
    /*
    cout_mutex.lock();
    std::cout << "Joined" << std::endl; // Debug
    cout_mutex.unlock();
    */
}

void AgentManager::stop_threads() {
    pauseThreads = false;
    endThreads = true;
    this->join_threads();
    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Stopped" << std::endl; // Debug
        cout_mutex.unlock();
    }
}

void AgentManager::createAndStartThreads() {
    unsigned maxLoopCount = 0;
    bool canSaveQtable;

    // Create threads in vector agentThreads.
    for ( unsigned i = 0; i != agentCount; i++) {
        // Only one thread can have its Qtable saved; the first one.
        if (i == 0) {
            canSaveQtable = true;
        } else {
            canSaveQtable = false;
        }
        agentThreads.emplace_back(agentTask, actors, sensors,
            agentShape, qtableFilename, drawGraphics,
            maxLoopCount, canSaveQtable);
    }

    // Evolution is used with generating new generations of Agents
    // if there is more than one Agent thread.
    if (agentCount == 1)
        useEvolutionGoal = false;
}

void AgentManager::initRun(unsigned runMode) {
    this->createAndStartThreads();

    // Every runMode controls the cleanup of threads.
    switch (runMode)
    {
    case 1: // Controll from command line.
    { // Separate the switch-statements.
        useLogging = true;

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
            std::cin >> userInput;
            command = userInput[0];
            switch (command)
            {
                case 'p':
                    this->pause_threads();
                    break;
                case 's':
                    this->saveQtable();
                    break;
                case 'r':
                    this->resume_threads();
                    break;
                case 'q':
                    this->stop_threads();
                    break;
            }
        }
    }
        break;
    case 2: // Controll from GUI (Not implemented)
        this->stop_threads();
        break;
    default:
        this->stop_threads();
        std::cout << "Invalid runMode: " << runMode << std::endl;
        break;
    }
}

void AgentManager::saveQtable() {
    // Saving is done when all the threads are paused.
    this->pause_threads();

    // Check that threre is some Agent threads.
    if (agentThreads.empty())
        throw std::runtime_error("There excists no Agent threads.");

    // Save the first agents Qtable.
    saveQtableInThread = true;

    // Wait untill the Qtable has been saved.
    while (saveQtableInThread) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    if (useLogging) {
        cout_mutex.lock();
        std::cout << "Saved\nPaused" << std::endl; // Debug
        cout_mutex.unlock();
    }
}

void AgentManager::evolveAgents() {
    // TODO
    // Implement after the big change of the types, and after the
    // most of the rest of the program has been implemented.
}
