#ifndef QLEARNING_9_SIMULATION_H_
#define QLEARNING_9_SIMULATION_H_


#include <Box2D/Box2D.h>
#include "interactor.hpp"
#include <vector>
#include <thread>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

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
    Simulation(unsigned& agentID,
        std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics);

    ~Simulation(){
        delete m_world;
    };

    /*
    Moves the Agent to the starting position in Box2D and returns the State.
    The State is used by the AgentLearner to have a matching starting position
    with the Simulation.
    */
    State moveAgentToBeginning();

    /*
    Receives an Action from the AgentLearner.
    Simulates the received Action in Box2D.
    The simulation takes a fixed defined amount of time.
    Returns the State of the Agent in Box2D after the simulation is done.
    The simulation waits after this for the next Action.
    */
    State simulateAction(Action& action);
private:
    FRIEND_TEST(test_Simulation, test_constructor);
    FRIEND_TEST(test_Simulation, test_simulateAction);

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape& agentShape;
    bool drawGraphics;

    b2World* m_world;
    b2Body* ground;
    b2Body* crawler;
    b2Body* forearm;
    b2Body* upperarm;
    b2RevoluteJoint* shoulder;
    b2RevoluteJoint* elbow;

    int shoulderMinAngle;
    int elbowMinAngle;
    int shoulderMaxAngle;
    int elbowMaxAngle;
    int shoulderID;
    int elbowID;

    std::thread testbedThread;
};

std::ostream& operator<<(std::ostream& os, Simulation const& simulation);

#endif
