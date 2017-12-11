#ifndef QLEARNING_9_SIMULATION_H_
#define QLEARNING_9_SIMULATION_H_

#include "interactor.hpp"
#include <vector>

/*
TODO: agentShape will be implemented according to the needs of the simulation.
It will also be moved to an other file. ATM: there is no time to implement
this feature, therefore AgentShape is a placeholder.
*/
typedef int AgentShape;

// Model-class used before the real Box2D simulation can be used.
// Shows the wanted behaviour.
class Simulation {
public:
    Simulation(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics)
        : actors(actors), sensors(sensors), agentShape(agentShape),
        drawGraphics(drawGraphics) {}

    std::vector<ResponsePacket> simulateAction(Action action)
    {
        // Dummy
        ResponsePacket responsePacket0(999, 1.2);
        ResponsePacket responsePacket1(1, 22.6);
        ResponsePacket responsePacket2(2, 52.3);
        return {responsePacket0, responsePacket1, responsePacket2};
    };
private:
	//these functions help to get and set values to crawler, aka communiate with box2d


	void createworld();
	//create world for crawler, drawgraphics goes possibly here

	void createcrawler();
	//creates crawler in world

	ResponsePacket getcrawlerstate(int responceid);
	//get current position og crawler joints and location

	void setcrawlerstate(ActorAction);
	//set crawler to turn joints to desired position

	void runstep();
	//run signle step in box2d simulation



    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    bool drawGraphics;
};

#endif



// main purpose is to provide mainframe for simulation and graphics





//create world and draw debug window





//place crawler in world






//start simulating crawler in step() function, and "talks" with agent manager

/*starts by sending current data of joints to agent manager in vector of pairs
*waits for learner to do its thing
*receives next wanted position of joints, and begins to move joints accordingly
*??waits couple of physics round to let joints achieve wated position??
*rinse and repeat
*/
