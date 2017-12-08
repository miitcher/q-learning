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
     * state-detecting sensor's possible states. Qtable is a map of
     * state-maps that contain actions for that state. The size of the Qtable
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

    void receiveSimulationResponse(State& state) {
        currentState = convertStateToKey(state);
    };

    // Chooses the best or a random action depending on the explorationFactor.
    // Communicates the choosen action to the simulation.
    Action doAction();
    /*
    {
        // Dummy
        ActorAction actionPacket0(0, Clockwise);
        ActorAction actionPacket1(1, Counterclockwise);
        //Action actionMessage = {actionPacket0, actionPacket1};
         return {actionPacket0, actionPacket1};
        //int const& key = _Qtable->getBestAction(currentState);
        //return convertKeyToAction(key);

    };
    */

    /* Acces functions */
    const int& getNumberOfStates() const { return numOfStates; };
    const int& getNumberOfMoves() const { return numOfMoves; };
    const std::vector<Actor>& getActors() const { return actors; };
    const std::vector<Sensor>& getSensors() const { return sensors; };
    const int& getState() const { return currentState; };

    const double& getDiscountFactor() const { return discountFactor; };
    const double& getLearningRate() const { return learningRate; };
    const double& getExplorationFactor() const { return explorationFactor; };

    void setDiscountFactor(double val) { discountFactor = val; };
    void setLearningRate(double val) { learningRate = val; };
    void setExplorationFactor(double val) { explorationFactor = val; };

    /* Write Qtable to an external file */
    void saveQtable() { _Qtable->saveToFile(); };

    /*
    Returns the Agents location on the x-axis in the simulation in
    the simulations units. This function is called after the Agent
    has resieved a response from the simulation.
    */
    const float& getXAxisLocation() const{
        // Dummy
        const float& ref = 10;
        return ref;
    }

private:
    FRIEND_TEST(test_AgentLearner, test_agents_actor);
    FRIEND_TEST(test_AgentLearner, test_constructor);
    FRIEND_TEST(test_AgentLearner, test_statekeys) ;
    FRIEND_TEST(test_AgentLearner, test_actionkeys) ;
    FRIEND_TEST(test_AgentLearner, test_agents_sensor);
    FRIEND_TEST(test_AgentLearner, test_quantizise);
    FRIEND_TEST(test_AgentLearner, test_doAction_and_chooseRandomAction);

    /* Next two methods initialize the statekeys*/

    /* stateKeys are in the format int stateKey = 141315 where each sensors
     * state is represented by two digits (14, 13, 15). Sensors' states are
     * in the range 1...quantizationSteps.
     */
    std::vector<int> createStateKeys1(std::vector<Sensor> const& sensors);

    /* actionKeys are in the format int actionKey = 30302 where each sensors
     * state is represented by two digits (3, 3, 2). Actors' actions are
     * enumeration + 1 in the key.
     */
    std::vector<int> createStateKeys2(std::vector<Sensor> const& sensors);

    /* Converts state to a key for the Qtable map*/
    int convertStateToKey(State const& state);

    /* Converts key from the qtable map to an Action
     */
    Action convertKeyToAction(int key);

    /* Helping method for key conversions:
     * Scales the sensorInput (double at the moment)
     * to integer from 0 to quantizationSteps.
     * I.e. from the range minAngle < double angle < maxAngle to
     * the range 0 < int index < quantizationSteps
     */
    int quantiziseSensorInput(int sensorID, SensorInput sInput);

    // to do anssi: the rest

    /* Calculates the updated Q-value using the reward, discoutFactor,
     * learningRate and the maximum Q-value of the next state's actions.

    void updateQtable(QState state, Move action, QState nextState);

     The response is the result of an action in the simulation.
    QReward& calcReward(std::vector<ResponsePacket> responseMessage);
    */

    Action chooseBestAction();
    Action chooseRandomAction();

    int ID;

    double discountFactor      = 0.9;  // range 0...1, e.g. 0.9, increase
    double learningRate        = 0.1;   // range: 0...1, e.g. 0.1
    double explorationFactor   = 0.5;   // range: 0...1, e.g. 0.5, decrease

    int currentState = 0;           // key to the current state
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    std::vector<int> stateKeys;
    std::vector<int> actionKeys;
    int numOfStates;
    int numOfMoves;
    Qtable* _Qtable;
};

std::ostream& operator<<(std::ostream& os, AgentLearner const& agent) ;

#endif
