#include "q-table.hpp"
#include <iostream>

Qtable::Qtable(int s, int a) :
        numberOfStates(s), numberOfActions(a) {
    // An empty Q-table will become a 1x1 sized Q-table. Hardly a problem?

    QValue initial = 0; // initial Q-value
    std::vector<QValue> actionVector(numberOfActions, initial);
    std::vector<QValue>& ref = actionVector;
    for(int i = 0; i < numberOfStates; i++){
        qValues.push_back(ref);
    }
}

const QValue& Qtable::getQvalue(const int& stateIndex,
        const int& actionIndex) const{
    QValue const& ref = qValues[stateIndex][actionIndex];
    return ref;
}


void Qtable::updateQvalue(const int& stateIndex, const int& actionIndex,
                             QValue& qValue){
    qValues[stateIndex][actionIndex] = qValue;
}


/* //to do: the rest of this class
QValue Qtable::getMaxQvalue(const QState& state){
    QValue zero = 0;
    QState s = state;
    Action a = qValues[state][0]
    size_t one = 1;
    if (qValues[state].size() < one) return zero;

    const QState& ref = state;
    QValue max = getQvalue(ref, qValues[state][0]);
    for (auto it : qValues[state]){
        if (it.getQvalue(state) > max){
            max = it.getQvalue(state);
        }
    }
    return max;
}


Action& Qtable::getRandomAction(const QState& state){

    Action ra = static_cast<Action>((rand() % numberOfActions));
    return ra;
}

Action& Qtable::getBestAction(const QState& state){
    Action best;
    for (auto it : state){
        if (it.getQvalue(state) > best){
            best = it;
        }
    }
    return best;
}
*/
