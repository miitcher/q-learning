#include "q-table.hpp"
#include <iostream>

Qtable::Qtable(int s, int a) :
        numberOfStates(s), numberOfActions(a) {
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

QValue Qtable::getMaxQvalue(int stateIndex){
    QValue zero = 0;
    size_t one = 1;
    if (qValues[stateIndex].size() < one) return zero;
              //returns 0 from empty --> to do Anssi: make better

    QValue max = getQvalue(0, 0);
    for (auto it : qValues[stateIndex]){
        if (it > max){
            max = it;
        }
    }
    return max;
}

int Qtable::getBestAction(int stateIndex){
    int best = 0;
    for (int i = 0; i < getNumberOfActions(); i++){
        if (getQvalue(stateIndex, i) > getQvalue(stateIndex, best)){
            best = i;
        }
    }
    return best;
}
