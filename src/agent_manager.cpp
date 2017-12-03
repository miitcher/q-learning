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

/*
Atomic booleans are thread safe, and they are used here to communicate
between threads.
*/
std::atomic_bool endThreads(false);
std::atomic_bool pauseThreads(false);

void agentTask(std::vector<Actor> actors, std::vector<Sensor> sensors,
    AgentShape agentShape, std::string qtableFilename,
    bool drawGraphics, unsigned maxLoopCount)
{
    //std::cout << "AgentTask started." << std::endl;

    Agent agent(actors, sensors, qtableFilename);
    Simulation simulation(actors, sensors, agentShape, drawGraphics);

    unsigned count = 0;
    // The learning and simulation parts communicate.
    while (true) {
        ActionMessage actionMessage = agent.doAction();
        ResponseMessage responseMessage = simulation.simulateAction(
            actionMessage);
        agent.receiveSimulationResponse(responseMessage);

        count++;
        if (maxLoopCount != 0 && count > maxLoopCount)
            break;

        /*
        // Debug
        if (count % int(pow(10, 5)) == 0) {
            cout_mutex.lock();
            std::cout << count / int(pow(10, 5)) << std::endl;
            cout_mutex.unlock();
        }
        std::cout << count << std::endl;
        */

        // Listen to commands from the main thread.
        while (pauseThreads)
            std::this_thread::sleep_for (std::chrono::seconds(1));
        if (endThreads)
            break;
    }

    //std::cout << "AgentTask is done." << std::endl;
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
    //std::cout << "Paused" << std::endl; // Debug
}

void AgentManager::resume_threads() {
    pauseThreads = false;
    //std::cout << "Resumed" << std::endl; // Debug
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
    //std::cout << "Joined" << std::endl; // Debug
}

void AgentManager::stop_threads() {
    pauseThreads = false;
    endThreads = true;
    this->join_threads();
    //std::cout << "Stopped" << std::endl; // Debug
}

void AgentManager::initRun(unsigned runMode) {
    for ( unsigned i = 0; i != agentCount; i++) {
        agentThreads.emplace_back(agentTask, actors, sensors,
            agentShape, qtableFilename, drawGraphics, 0);
    }

    switch (runMode)
    {
    case 0:
        // Smoketest for 100 milliseconds.
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
        this->stop_threads();
        break;
    case 1:
    { // Separate the switch-statements.
        // Controll from command line.
        cout_mutex.lock();
        std::cout   << "# " << agentCount << " Agents learning\n"
                    << "--------------------------------------\n"
                    << "Do things by typing a command and pressing enter.\n"
                    << "Unrecognized commands will be ignored.\n"
                    << "p: pause; r: resume; s: stop\n"
                    << "--------------------------------------" << std::endl;
        cout_mutex.unlock();

        // Take user input from commandline.
        std::string userInput;
        char command;
        while (command != 's') {
            std::cin >> userInput;
            command = userInput[0];
            switch (command)
            {
                case 'p':
                    this->pause_threads();
                    break;
                case 'r':
                    this->resume_threads();
                    break;
                case 's':
                    this->stop_threads();
                    break;
            }
        }
    }
        break;
    case 2:
        // Controll from graphical (Not implemented)
        this->stop_threads();
        break;
    }
}

void AgentManager::evolveAgents() {

}

void AgentManager::saveQvalues() {

}
