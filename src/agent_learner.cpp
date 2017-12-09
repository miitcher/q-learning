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

    // Differentiate the Location sensor from the state-detecting sensors.
    // Location sensor ID is 0
    std::vector<Sensor> stateDetectingSensors(sensors);
    for (std::vector<Sensor>::iterator it = stateDetectingSensors.begin(); it <
            stateDetectingSensors.end(); it++ ){
        if ((*it).getID() == 0){
            it = stateDetectingSensors.erase(it);
            break;
        }
    }

    // Initialize the Q-table:
    // Create stateKeys for each state

    int numOfstateDetectingSensors = stateDetectingSensors.size();

    // stateKeys for an agent with one state-detecting sensor
    if(numOfstateDetectingSensors == 1){
        stateKeys = createStateKeys1(sensors);
    }

    // stateKeys for an agent with two state-detecting sensors
    else if(numOfstateDetectingSensors == 2){
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

    // to to if needed: implement compatibility
    // for agents with more than 2 sensors or 3 actors

    // Initialize the Q-table
    qtable = Qtable(stateKeys, actionKeys, qtableFilename);
}

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors)
        : AgentLearner(actors, sensors, "") {}

// TODO Anssi
std::ostream& operator<<(std::ostream& os, AgentLearner const& agent){
    os << "AgentLearner, ID: " << agent.getID() << std::endl;
    os << "Actors" << std::endl;
    for (auto i : agent.getActors()){
        os << i << std::endl;
    }
    os << std::endl;
    os << "Sensors " << std::endl;
    for (auto i : agent.getSensors()){
        os << i << std::endl;
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

void AgentLearner::receiveSimulationResponse(State const& state){
    State copyState = state;
    previousLocation = location;
    for (State::iterator it = copyState.begin(); it < copyState.end(); it++ ){
        if ((*it).first == 0){
            location = (*it).second;
            it = copyState.erase(it);
            break;
        }
    }
    SensorInput distanceTravelled = (location - previousLocation);
    QReward reward = calculateReward(distanceTravelled);
    previousStateKey = currentStateKey;
    currentStateKey = convertStateToKey(copyState);
    int actionKey = qtable.getBestAction(currentStateKey);
    updateQtable(reward, actionKey);
}

void AgentLearner::updateQtable(QReward const& reward, int const& actionKey) {
    QValue oldQValue = qtable.getQvalue(previousStateKey, actionKey);
    QValue maxQvalueForCurrentState = qtable.getMaxQvalue(currentStateKey);

    QValue newQValue = ( ( 1- learningRate ) * oldQValue ) + learningRate *
                        ( reward * discountFactor * maxQvalueForCurrentState);
    qtable.updateQvalue(previousStateKey, actionKey, newQValue);
}

QReward AgentLearner::calculateReward
            (SensorInput const& distanceTravelled) const{
    SensorInput copyDistance = distanceTravelled;
    QReward casted =  static_cast<QReward>(copyDistance);
    return casted;
}

Action AgentLearner::chooseBestAction() {
    int actionKey = qtable.getBestAction(currentStateKey);
    return convertKeyToAction(actionKey);
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

    if (explorationFactor > randomFloat) { // which way: < oder > ?
        return chooseRandomAction();
    } else {
        return chooseBestAction();
    }
}
