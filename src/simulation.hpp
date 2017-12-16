#ifndef QLEARNING_9_SIMULATION_H_
#define QLEARNING_9_SIMULATION_H_


#include <Box2D/Box2D.h>
#include "interactor.hpp"
#include <vector>
#include <thread>


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

    b2World* m_world;

    Simulation(unsigned& agentID,
        std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics);

    /*
    Moves the Agent to the starting position in Box2D and returns the State.
    The State is used by the AgentLearner to have a matching starting position
    with the Simulation.
    */
    State moveAgentToBegining();

    /*
    Receives an Action from the AgentLearner.
    Simulates the received Action in Box2D.
    The simulation takes a fixed defined amount of time.
    Returns the State of the Agent in Box2D after the simulation is done.
    The simulation waits after this for the next Action.
    */
    State simulateAction(Action action);
private:
   
    //get current position of crawler joints and location,
    // sensorID IS NOT STATIC FOR THE JOINTS
    // Use same terms as in the other code!
    // The XAxisSensor has sensorID=999.
    ResponsePacket getcrawlerstate(int sensorID);

    //set crawler to turn joints to desired position
    void setCrawlerState(ActorAction);

    //run single step in box2d simulation
    // Explain what a step is!
    void runSimulationStep();

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    bool drawGraphics;

    std::thread testbedThread;
};

#endif
