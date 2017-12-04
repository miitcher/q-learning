#include "agent_learner.hpp"
#include "q-table.hpp"

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors,
        std::string const& qtableFilename)
        : actors(actors), sensors(sensors)
    {
    numOfStates = 1;
    numOfMoves = 1; // An empty Qtable becomes 1x1-sized (Improve?)
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
    // Initialize the Q-table
    //Qtable newQ(numOfStates, numOfMoves, qtableFilename);
    //Qtable _Qtable = newQ;
}

AgentLearner::AgentLearner(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors)
        : AgentLearner(actors, sensors, "") {}


// assumes 0-99 possible actions per actor
int AgentLearner::convertActionToKey(Action const& action){

    // factor determines the digits in the key that are used by one Actor
    int factor = 1;
    int key = 0;
    for (auto packet : action){
        key += factor * (packet.second +1);
        factor = factor *100;
    }
    return key;
}

// assumes 0-99 possible stateInputs per sensor
int AgentLearner::convertStateToKey(State const& state){

    // factor determines the digits in the key that are used by one Actor
    int factor = 1;
    int key = 0;
    for (auto packet : state){
        key += factor * (packet.second +1);
        factor = factor *100;
    }
    return key;
}

Action AgentLearner::convertKeyToAction(int key){
    Action m = {};
    ActorAction aPac;
    key--;
    for (auto it : getActors()){
        aPac.first = it.getID();
        aPac.second = static_cast<Move>(key % 100);
        key = (key/ 100) -1;
        m.push_back(aPac);
    }
    return m;
}
int AgentLearner::quantiziseSensorInput(Sensor& sensor, SensorInput sInput){

    if(sInput < sensor.getMinAngle() || sInput > sensor.getMaxAngle()){
        return -1; ////// to do Anssi: error message here?
    }
    SensorInput scaled = (sensor.getQuantizationSteps() *
                        (sInput - sensor.getMinAngle())) /
                        (sensor.getMaxAngle() - sensor.getMinAngle());
    return static_cast<int>(scaled);
}
/*
int AgentLearner::convertMoveToIndex
    (const std::vector<ActorAction>& actionPacs){

    // This nice loopety-loop generates a vector that contains each index to an
    // actor's vector of actions. i.e. the index of the action in that actors
    // action-vector that should be done.
    std::vector<int> indeces = {};
    int incr = 0;
    for (auto packet : actionPacs){
        for (auto actor : getActors()){
            if (packet.first == actor.getID()){
                for (auto action : actor.getMoves()){
                    if(action != packet.second){
                        incr++; // The index in the actor's action-vector
                    } else {
                        indeces.push_back(incr); // this vector contains indeces
                                            // for every actors' action-vector
                        incr = 0;
                        break;
                    }
                }
            }
        }
    }
    std::vector<int> numsOfMoves = {};
    for (auto actor : getActors()){
        numsOfMoves.push_back(actor.getNumberOfMoves());
    }

    for (auto i : indeces){std::cout << i << " ";}
        std::cout << std::endl;
    for (auto i : numsOfMoves){std::cout << i << " ";}
        std::cout << std::endl;

    int index = 0;
    int factor = 1;
    for (size_t i = 0; i < indeces.size(); i++){
        index += factor * indeces[i];
        factor = factor * numsOfMoves[i];

        for (auto i : indeces){std::cout << i << " ";}
        std::cout << std::endl;
        for (auto i : numsOfMoves){std::cout << i << " ";}
        std::cout << std::endl << "factor: " << factor<< std::endl <<"index: "
                                            << index << std::endl;

    }
    return index;
}

int AgentLearner::convertResponseToIndex
    (const std::vector<ResponsePacket>& responsePacs){

    // At the moment, this function assumes that all sensors contribute to the
    // aggregate state of the actual agent. Should be changed (todoAnssi)
    std::vector<int> indeces = {};
    int incr = 0;
    for (auto packet : responsePacs){
        for (auto sensor : getSensors()){
            if (packet.first == sensor.getID()){
                for (int state = 0; state < sensor.getQuantizationSteps()
                                        ; state++){
                    if(state != quantiziseSensorInput(sensor, packet.second)){
                        incr++;
                    }else{
                        indeces.push_back(incr);
                        incr = 0;
                        break;
                    }
                }
            }
        }
    }
    std::vector<int> numsOfStates = {};
    for (auto sensor : getSensors()){
        numsOfStates.push_back(sensor.getQuantizationSteps());
    }
    int index = 0;
    int factor = 1;
    for (size_t i = 0; i < indeces.size(); i++){
        index += factor * indeces[i];
        factor = factor * numsOfStates[i];
    }
    return index;
}

 To do Anssi: implement the rest

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
