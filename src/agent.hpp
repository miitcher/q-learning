#ifndef QLEARNING_9_AGENT_H_
#define QLEARNING_9_AGENT_H_

#include "q-table.hpp"
#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>

// The state and reward are calculated by the agent from the simulations
// resopnse.
typedef int QState;
typedef int QReward;
typedef int AgentShape; // TODO: agentShape will be implemented according
    // to the needs of the simulation.

class Agent {
public:
    Agent(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors);

    int convertSensorInputToInteger(SensorInput const& sInput);

    int convertActionToIndex(
        const std::vector<ActionPacket>& actionMessage);
    int convertResponseToIndex(
        const std::vector<ResponsePacket>& responseMessage);

    // Chooses the best or a random action
    std::vector<ActionPacket> chooseAction();

    void receiveSimulationResponse(
        std::vector<ResponsePacket> responseMessage);

    void updateQtable(QState state, Action action, QState nextState);

    QState getState();

    // The action tells what the agent tries to do in the simulation.
    void doAction(std::vector<ActionPacket> actionMessage);

    // The response is the result of an action in the simulation.
    QReward& calcReward(std::vector<ResponsePacket> responseMessage);

    const int& getNumberOfStates() const {return numOfStates;};
    const int& getNumberOfActions() const {return numOfActions;};
    const std::vector<Actor>& getActors() const {return actors;};
    const std::vector<Sensor>& getSensors() const {return sensors;};

    // read and write Qtable
    void saveQtable() { _Qtable->saveToFile(); };
    void loadQtable(std::string const& qtableFilename) {
        _Qtable->loadFromFile(qtableFilename);
    };
private:
    int ID;
    double discountFactor;      // range 0...1, e.g. 0.9, increase
    double learningRate;        // range: 0...1, e.g. 0.1
    double explorationFactor;   // range: 0...1, e.g. 0.5, decrease
    QState currentState;
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    int numOfStates;
    int numOfActions;
    Qtable* _Qtable;
};

#endif
