#include "agent_learner.hpp"
#include "q-table.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors,
        std::string const& qtableFilename)
        : actors(actors), sensors(sensors)
    {
    // Seed random number generator. Used when picking action (random or best).
    std::srand(std::time(0));

    // Calculate the total number of states
    numOfStates = 1;
    numOfMoves = 1;
    for (std::vector<Sensor>::const_iterator it = sensors.begin(); it <
            sensors.end(); it++ ){
        numOfStates = numOfStates * (*it).getQuantizationSteps();
    }
    // Calculate the total number of actions for each state
    for (std::vector<Actor>::const_iterator it = actors.begin(); it <
            actors.end(); it++ ){
        numOfMoves = numOfMoves * (*it).getQuantizationSteps();
    }

    // Initialize the Q-table:
    // Create stateKeys for each state

    // to do: exclude x axis sensor here
    int numOfSensors = sensors.size();

    // stateKeys for an agent with one state-detecting sensor
    if(numOfSensors == 1){
        stateKeys = createStateKeys1(sensors);
    }

    // stateKeys for an agent with two state-detecting sensors
    else if(numOfSensors == 2){
        stateKeys = createStateKeys2(sensors);
    }

    // Create actionKeys

    // actionKeys for an agent with one actor
    // e.g. 3
    if(actors.size()  == 1){
        int key = 1;
        for (auto move : actors[0].getMoves()){
            actionKeys.push_back(key + move);
        }
    }

    // actionKeys for an agent with two actors
    // e.g. 103
    else if(actors.size()  == 2){
        int key = 1;
        for (auto move1 : actors[1].getMoves()){
            key += 100*(move1 + 1);
            for (auto move2 : actors[0].getMoves()){
                actionKeys.push_back(key + move2);

            }
            key -= 100*(move1 + 1);
        }
    }

    // actionKeys for an agent with three actors
    // e.g. 10103
    else if(actors.size()  == 3){
        int key = 1;
        for (auto move1 : actors[2].getMoves()){
            key += 10000*(move1 + 1);
            for (auto move2 : actors[1].getMoves()){
                key += 100*(move2 + 1);
                for (auto move3 : actors[0].getMoves()){
                    actionKeys.push_back(key + move3);
                }
                key -= 100*(move2 + 1);
            }
            key -= 10000*(move1 + 1);
        }
    }

    // to to Anssi: implement compatibility for agents with more than 2 sensors
    // or 3 actors

    // Initialize the Q-table
    qtable = Qtable(stateKeys, actionKeys, qtableFilename);
}

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors)
        : AgentLearner(actors, sensors, "") {}

// TODO
void AgentLearner::receiveSimulationResponse(State& state) {
    currentState = convertStateToKey(state);
    State oldAnalogState = currentAnalogState;
    currentAnalogState = state;

    float revard = float(currentState ) / 10.0;

}

SensorInput AgentLearner::getXAxisLocation() {
    // The XAxisSensor has the id 0.
    for (auto responsePacket : currentAnalogState) {
        if (responsePacket.first == 0) {
            return responsePacket.second;
        }
    }

}

// TODO
void AgentLearner::updateQtable(QState state, Move action, QState nextState) {

}

// TODO Anssi
std::ostream& operator<<(std::ostream& os, AgentLearner const& agent){
    os << "Actor IDs ";
    for (auto i : agent.getActors()){
        os << i.getID() << "; " << i;
    }
    os << std::endl;
    os << "Sensor IDs ";
    for (auto i : agent.getSensors()){
        os << i.getID() << "; " << i;
    }
    os << std::endl;
    //os << "Location: " << agent.getXAxisLocation() << std::endl;
    os << "Key of the Current State: " << agent.getState() << std::endl;
    return os;
}

std::vector<int> AgentLearner::createStateKeys1
            (std::vector<Sensor> const& sensors) {
    std::vector<int> keys;
    //this is the element of two digits that represents the state or action
    int element = 100/sensors[0].getQuantizationSteps();
    int key = 0;
    // make steps so that the range from 0 to 100 is used
    for(int step = 0; step < sensors[0].getQuantizationSteps(); step++){
        key += element;
        stateKeys.push_back(key);
    }
    return keys;
}

std::vector<int> AgentLearner::createStateKeys2
                (std::vector<Sensor> const& sensors){
    std::vector<int> keys;
    int key = 0;
    for(int step = 0; step < sensors[1].getQuantizationSteps(); step++){
        key += 100;
        for(int i = 0; i < sensors[0].getQuantizationSteps(); i++){
            key++;
            keys.push_back(key);
        }
        key -= sensors[0].getQuantizationSteps();
    }
    return keys;
}

int AgentLearner::quantiziseSensorInput(int sensorID, SensorInput sInput){
    int input = static_cast<int>(sInput);
    int minAngle;
    int maxAngle;
    int quantizationSteps;
    // have to search the information of the sensor
    for (auto sensor: getSensors()){
        if (sensorID == sensor.getID()){
            minAngle = static_cast<int>(sensor.getMinAngle());
            maxAngle = static_cast<int>(sensor.getMaxAngle());
            quantizationSteps = sensor.getQuantizationSteps();
            break;
        }
    }
    // max angle is exclusive and min angle inclusive
    if(input < minAngle || input >= maxAngle){
        return -1; ////// to do Anssi: error message here?
    }
    int scaled = (quantizationSteps*(input-minAngle)) / (maxAngle - minAngle);
    return scaled;
}

// assumes 0-99 possible stateInputs per sensor
int AgentLearner::convertStateToKey(State const& state){

    // create the lowest key, e.g. 10101 if there's three
    int key = 0;
    for (unsigned i = 0; i < state.size(); i++){
        key += pow(100, i);
    }

    // factor determines the digits in the key that are used by one Actor
    int factor = 1;
    for (auto sensorState : state){
        int quantizised = quantiziseSensorInput
                                    (sensorState.first, sensorState.second);
        key += factor * quantizised;
        factor = factor *100;
    }
    return key;
}

Action AgentLearner::convertKeyToAction(int key){
    Action m = {};
    ActorAction actoraction;
    key--;
    for (auto it : getActors()){
        actoraction.first = it.getID();
        actoraction.second = static_cast<Move>(key % 100);
        key = (key/ 100) -1;
        m.push_back(actoraction);
    }
    return m;
}

// TODO
Action AgentLearner::chooseBestAction() {
    // Dummy
    ActorAction actionPacket0(0, Clockwise);
    ActorAction actionPacket1(1, Counterclockwise);
    return {actionPacket0, actionPacket1};
}

Action AgentLearner::chooseRandomAction() {
    Action action;
    for (auto actor : actors) {
        // Pick a random Move for every Actor.
        auto moves = actor.getMoves();
        Move randomMove = Move(std::rand() % moves.size());
        action.emplace_back(actor.getID(), randomMove);
        // Debug
        //std::cout<< actor << "\n\trandomMove: " << randomMove << std::endl;
    }
    return action;
}

Action AgentLearner::doAction() {
    // Generate random number from 0 to 1.
    float randomFloat = float(std::rand()) / float(RAND_MAX);
    // Debug
    //std::cout << "randomFloat: " << randomFloat
    //    << "\nexplorationFactor: " << explorationFactor << std::endl;

    if (explorationFactor < randomFloat) {
        return chooseRandomAction();
    } else {
        return chooseBestAction();
    }
}
