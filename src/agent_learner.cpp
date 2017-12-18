#include "agent_learner.hpp"
#include "q-table.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <ctime>

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors,
        std::string const& qtableFilename) :    actors(actors),
    sensors(sensors)

    {
    // Seed random number generator. Used when picking action (random or best).
    std::srand(std::time(0));

    if (sensors.size() < 1 || actors.size() < 1 ){
        throw std::invalid_argument("Invalid initialization of AgentLearner");
    }

    // Differentiate the Location sensor from the state-detecting sensors.
    // Location sensor ID is 999
    std::vector<Sensor> stateDetectingSensors(sensors);
    for (std::vector<Sensor>::iterator it = stateDetectingSensors.begin();
             it < stateDetectingSensors.end(); it++ ){
        if ((*it).getID() == 999){
            stateDetectingSensors.erase(it);
            break;
        }
    }

    // Initialize the Q-table:
    // Create stateKeys for each state

    int numOfstateDetectingSensors = stateDetectingSensors.size();

    // stateKeys for an agent with one state-detecting sensor
    if(numOfstateDetectingSensors == 1){
        stateKeys = createStateKeys1(stateDetectingSensors[0]);
        currentStateKey = stateKeys[0];
        previousStateKey = stateKeys[0];
    }

    // stateKeys for an agent with two state-detecting sensors
    else if(numOfstateDetectingSensors == 2){
        stateKeys = createStateKeys2(stateDetectingSensors);
        currentStateKey = stateKeys[0];
        previousStateKey = stateKeys[0];
    }

    // Create actionKeys

    // actionKeys for an agent with one actor
    // e.g. 3
    if(actors.size()  == 1){
        int key = 1;
        currentActionKey = key;
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
        currentActionKey = actionKeys[0];
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
        currentActionKey = actionKeys[0];
    }

    // to to if needed: implement compatibility
    // for agents with more than 2 sensors or 3 actors

    // Initialize the Q-table
    qtable = Qtable(stateKeys, actionKeys, qtableFilename);

/*
    for (auto move: actors[0].getMoves()){
        std::cout << move << std::endl;
    }
    for (auto move: actors[1].getMoves()){
        std::cout << move << std::endl;
    }
*/
}

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors)
        : AgentLearner(actors, sensors, "") {}

std::ostream& operator<<(std::ostream& os, AgentLearner const& agent){
    os << "AgentLearner, ID: " << agent.getID() << std::endl;

    os << "Discount factor: " << agent.getDiscountFactor() << std::endl;
    os << "Learning rate: " << agent.getLearningRate()<<std::endl;
    os << "Exploration factor: " << agent.getExplorationFactor()<<std::endl;
    os << "Actors:" << std::endl;
    for (auto i : agent.getActors()){
        os << i << std::endl;
    }
    os << std::endl;
    os << "Sensors:" << std::endl;
    for (auto i : agent.getSensors()){
        os << i << std::endl;
    }
    os << std::endl;
    os << "Location: " << agent.getXAxisLocation() << std::endl;
    os << "Key of the current State: " << agent.getState() << std::endl;
    os << "Key of the previous State: " << agent.getPreviousState()<<std::endl;
    return os;
}

std::vector<int> AgentLearner::createStateKeys1(Sensor const& sensor) {
    std::vector<int> keys = {};
    //this is the element of two digits that represents the state or action
    int key = 0;
    for(int step = 0; step < sensor.getQuantizationSteps(); step++){
        key++;
        keys.push_back(key);
    }
    return keys;
}

std::vector<int> AgentLearner::createStateKeys2
                (std::vector<Sensor> const& sensors) {
    std::vector<int> keys = {};
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
    float minAngle;
    float maxAngle;
    int quantizationSteps;
    // have to search the information of the sensor
    bool found = false;
    for (auto sensor: getSensors()){
        if (sensorID == sensor.getID()){
            minAngle = sensor.getMinAngle();
            maxAngle = sensor.getMaxAngle();
            quantizationSteps = sensor.getQuantizationSteps();
            found = true;
            break;
        }
    }
    if (!found){
        throw std::invalid_argument("Sensor ID does not belong to this agent");
    }
    // max angle is exclusive and min angle inclusive
    if(sInput < minAngle || sInput >= maxAngle){
        throw std::out_of_range("Sensor input is not within correct range: "
                     + std::to_string(minAngle) + " to " +
                       std::to_string(maxAngle) + ", input: " +
                                                std::to_string(sInput));
    }
    int scaled = static_cast<int>((quantizationSteps*(sInput-minAngle))
                                        / (maxAngle - minAngle));
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
    int quantizised;
    for (auto sensorState : state){
        try{
            quantizised = quantiziseSensorInput
                                (sensorState.first, sensorState.second);
        }
        catch(std::exception& e){
            std::cerr << "exception caught in convertStateToKey: "
                                << e.what() << '\n';
            quantizised = 1;
        }
        key += factor * quantizised;
        factor = factor *100;
    }
    return key;
}

Action AgentLearner::convertKeyToAction(int key){
    Action action = {};
    ActorAction actoraction;

    // Keys have additional units that need be taken off
    key--;

    // This reverses the key generation from Action
    for (auto it : getActors()){
        actoraction.first = it.getID();
        actoraction.second = static_cast<Move>(key % 100);
        key = (key/ 100) -1;
        action.push_back(actoraction);
    }
    return action;
}

void AgentLearner::receiveStartingState(State state) {
    // Implementation is a modified copy of receiveSimulationResponse.
    State copyState = state;
    try {
        // Differentiate the x-axis sensor
        for(State::iterator it = copyState.begin(); it < copyState.end(); it++){
            if ((*it).first == 999){
                location = (*it).second;
                previousLocation = location;
                it = copyState.erase(it);
                break;
            }
        }
        currentStateKey = convertStateToKey(copyState);
        previousStateKey = currentStateKey;
    } catch(std::exception& e) {
        std::cerr << "exception caught in receiveStartingState: "
                    << e.what() << '\n';
    }
}

void AgentLearner::receiveSimulationResponse(State& state) {
    State copyState = state;
    previousLocation = location;
    try {
        // Differentiate the x-axis sensor
        for(State::iterator it = copyState.begin(); it < copyState.end(); it++){
            if ((*it).first == 999){
                location = (*it).second;
                it = copyState.erase(it);
                break;
            }
        }
        // Calculate reward based on the distace travelled
        //std::cout << "distance: " << location - previousLocation <<std::endl;
        QReward reward = calculateReward( location - previousLocation );
        //std::cout << "reward: " << reward << std::endl;

        previousStateKey = currentStateKey;
        currentStateKey = convertStateToKey(copyState);

        updateQtable(reward);
    } catch (std::exception& e) {
        std::cerr << "exception caught in receiveSimulationResponse: "
                    << e.what() << '\n';
    }
}

void AgentLearner::updateQtable(QReward const& reward) {
    try{
        QValue oldQValue = qtable.getQvalue(previousStateKey, currentActionKey);

        QValue maxQvalueForCurrentState = qtable.getMaxQvalue(currentStateKey);

        QValue newQValue = ((1 - learningRate) * oldQValue) + learningRate *
                    ( reward * discountFactor * maxQvalueForCurrentState);

        qtable.updateQvalue(previousStateKey, currentActionKey, newQValue);

      // debug
  /*      std::cout   << "Old q-value: " << oldQValue << std::endl
                    << "max q-value for the next state: "
                    << maxQvalueForCurrentState << std::endl
                    << "reward: " << reward << std::endl
                    << "alpha, gamma: " << learningRate << ", "
                    << discountFactor << std::endl
                    << "new q-value: " << newQValue << std::endl<< std::endl;
*/
    }
    catch(std::exception& e){
        std::cerr << "exception caught in updateQtable: "
                    << e.what() << '\n';
    }
}

QReward AgentLearner::calculateReward(SensorInput distanceTravelled){
    // This should be adjusted to yield a reasonable value.
    // At the moment does only a type conversion

    //std::cout << "distace travelled: " << distanceTravelled << std::endl;

    return static_cast<QReward>(distanceTravelled + 1);
}

Action AgentLearner::chooseBestAction() {
    try{
        currentActionKey = qtable.getBestAction(currentStateKey);
        return convertKeyToAction(currentActionKey);
    }
    catch(std::exception& e){
        std::cerr << "exception caught in chooseBestAction: "
                    << e.what() << '\n';
        return convertKeyToAction(stateKeys[0]);
    }
}

Action AgentLearner::chooseRandomAction() {
    int randomIndex = std::rand() % actionKeys.size();
    currentActionKey = actionKeys[randomIndex];
    return convertKeyToAction(currentActionKey);
}

Action AgentLearner::doAction() {
    // Generate random number from 0 to 1.
    float randomFloat = float(std::rand()) / float(RAND_MAX);
    // Debug
    //std::cout << "randomFloat: " << randomFloat
    //    << "\nexplorationFactor: " << explorationFactor << std::endl;

    if (explorationFactor > randomFloat){
        return chooseRandomAction();
    } else {
        return chooseBestAction();
    }
}

// Has the Qtable save itself to file, with the given filename.
const std::string& AgentLearner::saveQtable(std::string filename) {
/*
        std::cout << "QVars: " << discountFactor << "; "
            << learningRate << "; " << explorationFactor << std::endl;
            */
    qtable.saveToFile(filename);
    return qtable.getQtableFilename();
}

// Has the Qtable save itself to file, with an generated unique filename.
const std::string& AgentLearner::saveQtable() {
/*
    std::cout << "QVars2: " << discountFactor << "; "
            << learningRate << "; " << explorationFactor << std::endl;
            */
    qtable.saveToFile();
    return qtable.getQtableFilename();
}
// Has the Qtable load itself from the given filename.
const std::string& AgentLearner::loadQtable(std::string filename) {
    qtable.loadFromFile(filename);
    return qtable.getQtableFilename();
}
// Has the Qtable load itself from the filename: Qtable.qtableFilename.
const std::string& AgentLearner::loadQtable() {
    qtable.loadFromFile();
    return qtable.getQtableFilename();
}
