#include "agent_manager.hpp"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

void printHello() {
    std::cout << "A thread started!" << std::endl;
    std::this_thread::sleep_for (std::chrono::seconds(1));
    std::cout << "A thread is done!" << std::endl;
}
/*
void agentThreadTask(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShapeType& agentShape, std::string const& qtableFilename) {
    std::cout << "A thread started!" << std::endl;
    std::cout << "A thread is done!" << std::endl;
}
*/
/*
void AgentManager::initRun() {

    auto t = std::thread(printHello);
    agentThreads.push_back(t);

    //std::vector<std::thread> agentThreads[agentCount] = std::thread(
    //    printHello); //, actors, sensors, agentShape, qtableFilename);

    std::cout << "Main thread is executing." << std::endl;

    for (auto t : agentThreads) {
        t.join();
        std::cout << "Thread joined." << std::endl;
    }
}
*/
