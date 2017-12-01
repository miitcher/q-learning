#include "agent.hpp"
#include "q-table.hpp"

Agent::Agent(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors) : actors(actors), sensors(sensors)
    {
    /* The Qtable is initialized from actors and sensors. Initializer needs
     * the number each actor's possible actions and the number of each
     * state-detecting sensor's possible states. Qtable is a vector of
     * state-vectors that contain actions for that state. The size of the Qtable
     * will be states * actions. If there is for example three kinds of actors:
     * actors with 3, 2 and 2 possible actions, for each state there is
     * 3 * 2 * 2 = 12 possible actions. If three state-detecting sensors can
     * detect each 10 different states, the total number of states is
     * 10³ = 1000. This would make a Qtable of a size 1000 * 12.
     */
    numOfStates = 1; numOfActions = 1;
    // Calculate the total number of states
    for (std::vector<Sensor>::const_iterator it = sensors.begin(); it <
            sensors.end(); it++ ){
        numOfStates = numOfStates * (*it).getQuantizationSteps();
    }
    // Calculate the total number of actions for each state
    for (std::vector<Actor>::const_iterator it = actors.begin(); it <
            actors.end(); it++ ){
        numOfActions = numOfActions * (*it).getQuantizationSteps();
    }
    // Initialize the Q-table
    Qtable newQ(numOfStates, numOfActions);
    Qtable _Qtable = newQ;
}

int Agent::convertActionToIndex
    (const std::vector<ActionPacket>& actionPacs){
    /* This function converts a vector of actions for individual actors of the
     * agent to a single action for the agent, which is an index to the Q-table.
     */

    // This nice loopety-loop generates a vector that contains each index to an
    // actor's vector of actions. i.e. the index of the action in that actors
    // action-vector that should be done.
    std::vector<int> indeces = {};
    int incr = 0;
    for (auto packet : actionPacs){
        for (auto actor : getActors()){
            if (packet.first == actor.getID()){
                for (auto action : actor.getActions()){
                    if(action != packet.second){
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
    std::vector<int> numsOfActions = {};
    for (auto actor : actors){
        numsOfActions.push_back(actor.getNumberOfActions());
    }
/*
    for (auto i : indeces){std::cout << i << " ";}
        std::cout << std::endl;
    for (auto i : numsOfActions){std::cout << i << " ";}
        std::cout << std::endl;
*/
    int index = 0;
    int factor = 1;
    for (size_t i = 0; i < indeces.size(); i++){
        index += factor * indeces[i];
        factor = factor * numsOfActions[i];
    /*
        for (auto i : indeces){std::cout << i << " ";}
        std::cout << std::endl;
        for (auto i : numsOfActions){std::cout << i << " ";}
        std::cout << std::endl << "factor: " << factor<< std::endl <<"index: "
                                            << index << std::endl;
    */
    }
    return index;
}

int Agent::convertSensorInputToInteger(SensorInput const& sInput){
    // To do Anssi: scale the input to integer from 0 to quantizationSteps
    // i.e. from the scale minAngle < double angle < maxAngle to
    // the scale 0 < int index < quantizationSteps
    return static_cast<int>(sInput);
}

int Agent::convertResponseToIndex
    (const std::vector<ResponsePacket>& responsePacs){
    /* This function converts a vector of states for individual sensors of the
     * agent to a single state for the agent, which is an index to the Q-table.
     */
    std::vector<int> indeces = {};
    int incr = 0;
    for (auto packet : responsePacs){
        for (auto sensor : getSensors()){
            if (packet.first == sensor.getID()){
                for (int state = 0; state < sensor.getQuantizationSteps()
                                        ; state++){
                    if(state != convertSensorInputToInteger(packet.second)){
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

/* To do Anssi: implement the rest

std::vector<ActionPacket> Agent::chooseAction(){

}

void Agent::updateQtable(QState state,
        Action action, QState nextState){

}

void Agent::doAction(std::vector<ActionPacket> actionMessage){

}

QReward& Agent::calcReward
    (std::vector<ResponsePacket> responseMessage){

}

void Agent::saveQtable() { _Qtable->saveToFile(){

}

void Agent::loadQtable(std::string const& qtableFilename){

}
*/
