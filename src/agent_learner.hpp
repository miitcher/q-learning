#ifndef QLEARNING_9_AGENT_H_
#define QLEARNING_9_AGENT_H_

#include "q-table.hpp"
#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>

// Add these two lines and FRIEND_TEST declarations in private to test
// private methods
#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

// The state and reward are calculated by the agent from the simulation's
// response.
typedef int QState;
typedef int QReward;
typedef int AgentShape; // TODO: agentShape will be implemented according
    // to the needs of the simulation. It will also be moved to an other file.

class AgentLearner {
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
    AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors,
        std::string const& qtableFilename);

    // Construct AgentLearner with qtableFilename as "".
    AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors);

    // Unpack the response - use quantisizeSensorInput
    void receiveSimulationResponse(
        std::vector<ResponsePacket>& state) {
        // Dummy
        currentState += state[0].first;
    };

    // Communicates the choosen action to the simulation.
    Action doAction() {
        // Dummy
        ActorAction actionPacket0(0, Clockwise);
        ActorAction actionPacket1(1, Counterclockwise);
        //Action actionMessage = {actionPacket0, actionPacket1};
        return {actionPacket0, actionPacket1};
    };


    /* Acces functions */
    const int& getNumberOfStates() const { return numOfStates; };
    const int& getNumberOfMoves() const { return numOfMoves; };
    const std::vector<Actor>& getActors() const { return actors; };
    const std::vector<Sensor>& getSensors() const { return sensors; };
    const QState& getState() const { return currentState; };

    /* Write Qtable to an external file */
    void saveQtable() { _Qtable->saveToFile(); };

    /*
    Returns the Agents location on the x-axis in the simulation in
    the simulations units. This function is called after the Agent
    has resieved a response from the simulation.
    */
    float getXAxisLocation() {
        // Dummy
        return 10.0;
    }

private:
    FRIEND_TEST(test_AgentLearner, test_agents_actor);
    FRIEND_TEST(test_AgentLearner, test_constructor);
    FRIEND_TEST(test_AgentLearner, test_agents_sensor);
    FRIEND_TEST(test_AgentLearner, test_quantizise);
    FRIEND_TEST(test_AgentLearner, test_key_conversion);


    /* Converts Action to a key for the Qtable map*/
    int convertActionToKey(Action const& action);

    /* Converts state to a key for the Qtable map*/
    int convertStateToKey(State const& state);

    /* Converts key from the qtable map to an Action*/
    Action convertKeyToAction(int key);

    /* Scales the sensorInput (double at the moment)
     * to integer from 0 to quantizationSteps.
     * I.e. from the range minAngle < double angle < maxAngle to
     * the range 0 < int index < quantizationSteps
     */
    int quantiziseSensorInput(Sensor& sensor, SensorInput sInput);

    /* Calculates the updated Q-value using the reward, discoutFactor,
     * learningRate and the maximum Q-value of the next state's actions.

    void updateQtable(QState state, Move action, QState nextState);

     The response is the result of an action in the simulation.
    QReward& calcReward(std::vector<ResponsePacket> responseMessage);

     Chooses the best or a random action depending on the explorationFactor
    std::vector<Actor> chooseMove();*/

    int ID;
    double discountFactor;      // range 0...1, e.g. 0.9, increase
    double learningRate;        // range: 0...1, e.g. 0.1
    double explorationFactor;   // range: 0...1, e.g. 0.5, decrease
    QState currentState;
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    int numOfStates;
    int numOfMoves;
    Qtable* _Qtable;

    // Probably won't need these anymore

    // The next two functions are used only at initialization
    // because they are slow.
    // To do Anssi: create a dictionary structure using these functions.
    // The dictionary is fast and is used during the learning process.

    /* Converts a vector of actions of individual actors
     * to a single action of the agent which is an index to the Q-table.

    int convertMoveToIndex(
        const std::vector<ActorAction>& actionMessage);

    Converts a vector of states of individual sensors
     * to a single state of the agent which is an index to the Q-table.

    int convertResponseToIndex(
        const std::vector<ResponsePacket>& responseMessage);
*/
};

#endif
