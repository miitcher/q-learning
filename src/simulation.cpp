#include "simulation.hpp"
#include "interactor.hpp"
#include <vector>
#include <stdexcept>

#include <thread>
#include <iostream>
#include <sstream>
#include "Testbed/Framework/TestbedMain.h"

//NEED to iclude required framework for box2d
//HEAVILY unfinished, lots to do!

void helloThere() {
    std::cout << "Simulation: hello" << std::endl;
}

// TODO: This should be done better inside the Simulation-class.
void runTestbed(unsigned agentID) {
    std::string agentID_string = std::to_string(agentID);
    std::cout << "agentID_string: " << agentID_string << std::endl;
    // Max 100 agent-threads.
    if (agentID_string.size() > 2) {
        throw std::runtime_error("Too many agent-threads.");
    }
    char he[3] = {agentID_string[0], agentID_string[1], '\0'};

    //char* argv[] = {"Hello"};
    char* argv[] = {he};
    TestbedMain(1, argv);
}

Simulation::Simulation(unsigned& agentID,
    std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    drawGraphics(drawGraphics)
{
    // TODO
    // Running Testbed as a thread or not, will make different problems come up.

    // Running simulation as a thread.
    /*
    ERROR:
        freeglut (1): illegal glutInit() reinitialization attempt
        X Error of failed request:  BadWindow (invalid Window parameter)
          Major opcode of failed request:  4 (X_DestroyWindow)
          Resource id in failed request:  0x0
          Serial number of failed request:  16
          Current serial number in output stream:  19
    */
    //std::thread testbedThread(runTestbed, agentID);
    //testbedThread.join();

    //ERROR1 is caused by calling testbed twice, it appears to initialize freeglut twice and freeglut doest like that

    // Just running the simulation
    /*
    ERROR:
        [xcb] Unknown request in queue while dequeuing
        [xcb] Most likely this is a multi-threaded client and XInitThreads has not been called
        [xcb] Aborting, sorry about that.
        main: ../../src/xcb_io.c:165: dequeue_pending_request: Assertion `!xcb_xlib_unknown_req_in_deq' failed.
    */

//testcode that compiles and runs
//if (agentID == 0){
//    runTestbed(agentID);
//}
}

State Simulation::moveAgentToBegining() {
    // TODO: Dummy
    ResponsePacket responsePacket0(999, 1.2);
    ResponsePacket responsePacket1(1, 22.6);
    ResponsePacket responsePacket2(2, 52.3);
    return {responsePacket0, responsePacket1, responsePacket2};
}

State Simulation::simulateAction(Action action) {
    // TODO: Dummy
    ResponsePacket responsePacket0(999, 1.2);
    ResponsePacket responsePacket1(1, 22.6);
    ResponsePacket responsePacket2(2, 52.3);
    return {responsePacket0, responsePacket1, responsePacket2};
}

/*
// USE 4 SPACES INSTEAD OF TABS!

//get current position of crawler joints and location
ResponsePacket getcrawlerstate(int responseid){
    float elbowangle = elbow->GetJointAngle();
    float shoulderangle = shoulder->GetJointAngle();
    float crawlerposition = 0; //TODO get crawler position working
    //actorid 1 = shoulder, 2 = elbow, 999 = position(x-coordinate);
    if(responseid == 1){
    ResponsePacket shoulderresponce(1,shoulderangle);
    return shoulderresponce;
    }
    if(responseid == 2){
    ResponsePacket elbowresponce(2,elbowangle);
    return elbowresponce;
    }
    if(responseid == 999){
    ResponsePacket locationresponce(999,crawlerposition);
    return locationresponce;
    }
    else{
    //returns empty responsepacket if called wrong responceid
    return ResponcePacket(0,0);
    }
}
*/
