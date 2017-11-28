#include "learning.hpp"
#include "q-table.hpp"
#include <iostream>


Qtable::Qtable(std::vector<Actor> const& actors,
        std::vector<Sensor> const& sensors) :
        numberOfStates(1), numberOfActions(1) {
    // An empty Q-table will become a 1x1 sized Q-table. Hardly a problem?

    // Calculate the total number of states
    for (std::vector<Sensor>::const_iterator it = sensors.begin(); it <
            sensors.end(); it++ ){
        numberOfStates = numberOfStates * (*it).getQuantizationSteps();
    }

    // Calculate the total number of actions for each state
    for (std::vector<Actor>::const_iterator it = actors.begin(); it <
            actors.end(); it++ ){
        numberOfActions = numberOfActions * (*it).getQuantizationSteps();
    }
    QvalueType initial = 0; // initial Q-value
    std::vector<QvalueType> actionVector(numberOfActions, initial);
    std::vector<QvalueType>& ref = actionVector;
    for(int i = 0; i < numberOfStates; i++){
        qValues.push_back(ref);
    }
};

QvalueType Qtable::getQvalue(StateType const& state, ActionType const& action){
    return qValues[state][action];
}


void Qtable::updateQvalue(const StateType& state, const ActionType& action,
                QvalueType& qValue){
    qValues[state][action] = qValue;
}

/* //to do: the rest of this class
ActionType& Qtable::getRandomAction(const StateType& state){

    ActionType ra = static_cast<ActionType>((rand() % numberOfActions));
    return ra;
}

ActionType& Qtable::getBestAction(const StateType& state){
    ActionType best;
    for (auto it : state){
        if (it.getQvalue(state) > best){
            best = it;
        }
    }
    return best;
}

QvalueType& Qtable::getMaxQvalue(const StateType& state){
    qValueType& zero = 0;
    if (state.length() < 1) return zero;

    QvalueType& max = getQvalue(state[0]);
    for (auto it : state){
        if (it.getQvalue(state) > max){
            max = it.getQvalue(state);
        }
    }
    return max;
}

*/
