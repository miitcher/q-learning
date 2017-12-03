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

//std::mutex cout_mutex;

void do_join(std::thread& t) { t.join(); }

void join_all(std::vector<std::thread>& v) {
    std::for_each(v.begin(), v.end(), do_join);
}

// Remove later
void printHello() {
    //cout_mutex.lock();
    std::cout << "A thread started!" << std::endl;
    //cout_mutex.unlock();
    std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "A thread is done!" << std::endl;
}

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

        // Debug
        if (count % int(pow(10, 5)) == 0) {
            std::cout << count / int(pow(10, 5)) << std::endl;
        }

        // Here would be a good place to have a listener that
        // can do stuff.
    }

    //std::cout << "AgentTask is done." << std::endl;
}

void AgentManager::initRun() {
    agentCount = 3;
    for ( unsigned i = 0; i != agentCount; i++) {
        //agentThreads.emplace_back(printHello); // WORKED
        agentThreads.emplace_back(agentTask, actors, sensors,
            agentShape, qtableFilename, drawGraphics, 0);
    }

    std::cout << agentCount << " threads executing." << std::endl;

    std::this_thread::sleep_for (std::chrono::seconds(5));

    join_all(agentThreads);
}
