#include "simulation.hpp"
#include "interactor.hpp"
#include <vector>

//NEED to iclude required framework for box2d
//HEAVILY unfinished, lots to do!

Simulation::Simulation(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    drawGraphics(drawGraphics) {}

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
	return ResponcePacket(0,0); //returns empty responsepacket if called wrong responceid
	}
}
*/