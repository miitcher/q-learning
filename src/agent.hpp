#ifndef QLEARNING_9_AGENT_H_
#define QLEARNING_9_AGENT_H_

#include "q-table.hpp"
#include <vector>
#include <string>
#include <iostream>

// The state and reward are calculated by the agent from the simulations
// resopnse.
typedef int StateType;
typedef int RewardType;
typedef int AgentShapeType; // TODO: agentShape will be implemented according
    // to the needs of the simulation.

class Agent {
public:
    Agent(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors);

    int convertActionToIndex(const std::vector<ActionPacketType>& action);

    // Chooses the best or a random action
    ActionType& chooseAction(StateType state);

    void updateQtable(StateType state, ActionType action, StateType nextState);

    StateType getState();

    // The action tells what the agent tries to do in the simulation.
    void doAction(std::vector<ActionPacketType> actionMessage);

    // The response is the result of an action in the simulation.
    RewardType& calcReward(std::vector<ResponsePacketType> responseMessage);

    int getNumberOfStates(){return numOfStates;};
    int getNumberOfActions(){return numOfActions;};
    std::vector<Actor> getActors(){return actors;};
    std::vector<Sensor> getSensors(){return sensors;};

private:
    int ID;
    double discountFactor;      // range 0...1, e.g. 0.9, increase
    double learningRate;        // range: 0...1, e.g. 0.1
    double explorationFactor;   // range: 0...1, e.g. 0.5, decrease
    StateType currentState;
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    int numOfStates;
    int numOfActions;
    Qtable* _Qtable;
};

#endif
