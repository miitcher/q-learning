#ifndef QLEARNING_9_AGENT_H_
#define QLEARNING_9_AGENT_H_

#include "q-table.hpp"
#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>

// The state and reward are calculated by the agent from the simulation's
// response.
typedef int QState;
typedef int QReward;
typedef int AgentShape; // TODO: agentShape will be implemented according
    // to the needs of the simulation.

class Agent {
public:
    /* The Qtable is initialized from actors and sensors. Initializer needs
     * the number each actor's possible actions and the number of each
     * state-detecting sensor's possible states. Qtable is a vector of
     * state-vectors that contain actions for that state. The size of the Qtable
     * will be states * actions. If an agent has, for example, three actors
     * with 3, 2 and 2 possible actions, for each state there is
     * 3 * 2 * 2 = 12 possible actions. If three state-detecting sensors can
     * detect each 10 different states, the total number of states is
     * 10³ = 1000. This would make a Qtable of a size 1000 * 12.
     */
    Agent(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors);

    // Unpack the response - use convertSensorInputToInteger
    void receiveSimulationResponse(
        std::vector<ResponsePacket> responseMessage);

    /* This function communicates the action to the simulation. */
    ActionMessage doAction(std::vector<ActionPacket> actionMessage);

    /* Acces functions */
    const int& getNumberOfStates() const { return numOfStates; };
    const int& getNumberOfActions() const { return numOfActions; };
    const std::vector<Actor>& getActors() const { return actors; };
    const std::vector<Sensor>& getSensors() const { return sensors; };
    const QState& getState() const { return currentState; };

    /* Read Qtable from - and write Qtable to - an external file */
    void saveQtable() { _Qtable->saveToFile(); };
    void loadQtable(std::string const& qtableFilename) {
        _Qtable->loadFromFile(qtableFilename);
    };

private:
    FRIEND_TEST(test_Agent, test_agents_actor); // needed for testing
    FRIEND_TEST(test_Agent, test_constructor);  // private methods
    FRIEND_TEST(test_Agent, test_agents_sensor);
    FRIEND_TEST(test_Agent, test_quantizise);

    // The next two functions are used only at initialization
    // because they are slow.
    // To do Anssi: create a dictionary structure using these functions.
    // The dictionary is fast and is used during the learning process.

    /* This function converts a vector of actions of individual actors
     * to a single action of the agent which is an index to the Q-table.
     */
    int convertActionToIndex(
        const std::vector<ActionPacket>& actionMessage);

    /* This function converts a vector of states of individual sensors
     * to a single state of the agent which is an index to the Q-table.
     */
    int convertResponseToIndex(
        const std::vector<ResponsePacket>& responseMessage);

    /* This function scales the sensorInput (double at the moment)
     * to integer from 0 to quantizationSteps.
     * I.e. from the scale minAngle < double angle < maxAngle to
     * the scale 0 < int index < quantizationSteps
     */
    int quantiziseSensorInput(Sensor& sensor, SensorInput sInput);

    /* Calculates the updated Q-value using the reward, discoutFactor,
     * learningRate and the maximum Q-value of the next state's actions.
     */
    void updateQtable(QState state, Action action, QState nextState);

    /* The response is the result of an action in the simulation. */
    QReward& calcReward(std::vector<ResponsePacket> responseMessage);

    /* Chooses the best or a random action depending on the explorationFactor */
    std::vector<ActionPacket> chooseAction();

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
