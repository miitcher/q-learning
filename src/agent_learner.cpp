#include "agent_learner.hpp"
#include "q-table.hpp"
#include <cmath>

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors,
        std::string const& qtableFilename)
        : actors(actors), sensors(sensors)
    {
    numOfStates = 1;
    numOfMoves = 1;
    // Calculate the total number of states
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
    Qtable newQ(stateKeys, actionKeys, qtableFilename);
    Qtable _Qtable = newQ;
}

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors)
        : AgentLearner(actors, sensors, "") {}

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
    for (int i = 0; i < state.size(); i++){
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

/* to do anssi : the rest
std::vector<ActorAction> AgentLearner::chooseMove(){

}


void AgentLearner::updateQtable(QState state,
        Move action, QState nextState){

}

void AgentLearner::doAction(std::vector<ActorAction> actionMessage){

}

QReward& AgentLearner::calcReward
    (std::vector<ResponsePacket> responseMessage){

}

void AgentLearner::saveQtable() { _Qtable->saveToFile(){

}

void AgentLearner::loadQtable(std::string const& qtableFilename){

}
*/
