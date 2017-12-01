#include "agent_manager.hpp"
#include "agent.hpp"
#include "interactor.hpp"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

void printHello() {
    std::cout << "A thread started!" << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "A thread is done!" << std::endl;
}

void agentThreadTask(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, std::string const& qtableFilename,
    bool drawGraphics)
{
    std::cout << "Thread started!" << std::endl;

    Agent agent(actors, sensors);
    Simulation simulation(actors, sensors, agentShape, drawGraphics);

    // The learning and simulation parts communicate.
    /*
    while (true) {
        std::vector<ActionPacket> actionMessage = agent.chooseAction();
        std::vector<ResponsePacket> responseMessage
            = simulation.simulateAction(actionMessage);
        agent.receiveSimulationResponse(responseMessage);
    }
    */

    std::cout << "Thread is done!" << std::endl;
}

void AgentManager::initRun() {
    /*
    for ( auto i = 0; i == agentCount; i++) {
        agentThreads.push_back(
            std::thread(printHello)
        );
    }

    //std::vector<std::thread> agentThreads[agentCount] = std::thread(
    //    printHello); //, actors, sensors, agentShape, qtableFilename);

    std::cout << agentCount << " threads executing." << std::endl;

    // Waiting on user input.
    std::string uin;
    std::cin >> uin;

    for (auto t : agentThreads) {
        t.join();
        std::cout << "Thread joined." << std::endl;
    }
    */
}
