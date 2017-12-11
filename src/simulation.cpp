#include "simulation.hpp"
#include "interactor.hpp"
#include <vector>

//NEED to iclude required framework for box2d
//HEAVILY unfinished, lots to do! 

/*
    Simulation(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics)
        : actors(actors), sensors(sensors), agentShape(agentShape),
        drawGraphics(drawGraphics) {}
*/

/*
//create world for crawler, drawgraphics goes possibly here
void createworld();{        
}

//creates crawler in world
void createcrawler();{      
}

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
	
//set crawler to turn joints to desired position
void setcrawlerstate(ActorAction);{  
}

//run signle step in box2d simulation
void runstep();{             	
}
*/
