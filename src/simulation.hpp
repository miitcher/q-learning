#ifndef QLEARNING_9_SIMULATION_H_
#define QLEARNING_9_SIMULATION_H_

#include <Box2D/Box2D.h>
#include "interactor.hpp"
#include <vector>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

/*
TODO: agentShape will be implemented according to the needs of the simulation.
It will also be moved to an other file. ATM: there is no time to implement
this feature, therefore AgentShape is a placeholder.
*/
typedef int AgentShape;

/* Uses box2d to simulate the process */
class Simulation {
public:
    Simulation(unsigned& agentID,
        std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShape& agentShape, bool drawGraphics);

    ~Simulation(){
        delete m_world;
    };

    /*
    Returns the Box2D State.
    */
    State getState();

    /**
    The Agent is moved stright to the starting position.
    This moving disregards the physics. The moving is practically the Agent
    teleporting to the starting position, with its arm in the starting
    position in a resting state (velosity is zero).
    This function used by the AgentManager when evolving Agents.
    */
    void moveAgentToStartPosition();

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



    float timeStep;
    int velocityIterations;
    int positionIterations;
    float shoulderMinAngle;
    float elbowMinAngle;
    float shoulderMaxAngle;
    float elbowMaxAngle;
    float shoulderqsteps;
    int shoulderID;
    int elbowID;
    int simsteps;
};

std::ostream& operator<<(std::ostream& os, Simulation const& simulation);

#endif
