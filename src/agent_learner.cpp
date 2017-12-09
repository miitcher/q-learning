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
    int input = static_cast<int>(sInput);
    int minAngle;
    int maxAngle;
    int quantizationSteps;
    // have to search the information of the sensor
    bool found = false;
    for (auto sensor: getSensors()){
        if (sensorID == sensor.getID()){
            minAngle = static_cast<int>(sensor.getMinAngle());
            maxAngle = static_cast<int>(sensor.getMaxAngle());
            quantizationSteps = sensor.getQuantizationSteps();
            found = true;
            break;
        }
    }
    if (!found){
        throw std::invalid_argument("this sensor doesn't belong to this agent");
    }
    // max angle is exclusive and min angle inclusive
    if(input < minAngle || input >= maxAngle){
        throw std::invalid_argument("Sensor input is not within correct range");
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
    int quantizised;
    for (auto sensorState : state){
        try{
            quantizised = quantiziseSensorInput
                                (sensorState.first, sensorState.second);
        }catch(std::exception& e){
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

void AgentLearner::receiveSimulationResponse(State const& state){

    State copyState = state;
    previousLocation = location;
    try{
        // Differentiate the x-axis sensor
        for(State::iterator it = copyState.begin(); it < copyState.end(); it++){
            if ((*it).first == 999){
                location = (*it).second;
                it = copyState.erase(it);
                break;
            }
        }
        // Assess the location
        SensorInput distanceTravelled = (location - previousLocation);
        QReward reward = calculateReward(distanceTravelled);
        previousStateKey = currentStateKey;
        currentStateKey = convertStateToKey(copyState);

        // best possible QValue of current state is used to calculate the new
        // Q-value of the previous state-action element
        int actionKey = qtable.getBestAction(currentStateKey);
        updateQtable(reward, actionKey);
    }catch(std::exception& e){
        std::cerr << "exception caught in updateQtable: "
                    << e.what() << '\n';
    }
}

void AgentLearner::updateQtable(QReward const& reward, int const& actionKey) {
    try{
        QValue oldQValue = qtable.getQvalue(previousStateKey, actionKey);

        QValue maxQvalueForCurrentState = qtable.getMaxQvalue(currentStateKey);

        QValue newQValue = ((1 - learningRate) * oldQValue) + learningRate *
                    ( reward * discountFactor * maxQvalueForCurrentState);
        qtable.updateQvalue(previousStateKey, actionKey, newQValue);
    }catch(std::exception& e){
        std::cerr << "exception caught in updateQtable: "
                    << e.what() << '\n';
    }
}

QReward AgentLearner::calculateReward
            (SensorInput distanceTravelled) const{

    // This should be adjusted to yield a reasonable value.
    // At the moment does only a type conversion
    SensorInput copyDistance = distanceTravelled;
    QReward casted =  static_cast<QReward>(copyDistance);
    return casted;
}

Action AgentLearner::chooseBestAction() {
    try{
        int actionKey = qtable.getBestAction(currentStateKey);
        return convertKeyToAction(actionKey);
    }catch(std::exception& e){
        std::cerr << "exception caught in chooseBestAction: "
                    << e.what() << '\n';
        return convertKeyToAction(stateKeys[0]);
    }
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
