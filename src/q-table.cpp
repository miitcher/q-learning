#include "q-table.hpp"
#include <iostream>

Qtable::Qtable(int s, int a) :
        numberOfStates(s), numberOfActions(a) {
    // An empty Q-table will become a 1x1 sized Q-table. Hardly a problem?

    QvalueType initial = 0; // initial Q-value
    std::vector<QvalueType> actionVector(numberOfActions, initial);
    std::vector<QvalueType>& ref = actionVector;
    for(int i = 0; i < numberOfStates; i++){
        qValues.push_back(ref);
    }
}

const QvalueType& Qtable::getQvalue(const int& stateIndex,
        const int& actionIndex) const{
    QvalueType const& ref = qValues[stateIndex][actionIndex];
    return ref;
}


void Qtable::updateQvalue(const int& stateIndex, const int& actionIndex,
                             QvalueType& qValue){
    qValues[stateIndex][actionIndex] = qValue;
}


/* //to do: the rest of this class
QvalueType Qtable::getMaxQvalue(const StateType& state){
    QvalueType zero = 0;
    StateType s = state;
    ActionType a = qValues[state][0]
    size_t one = 1;
    if (qValues[state].size() < one) return zero;

    const StateType& ref = state;
    QvalueType max = getQvalue(ref, qValues[state][0]);
    for (auto it : qValues[state]){
        if (it.getQvalue(state) > max){
            max = it.getQvalue(state);
        }
    }
    return max;
}


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
*/
