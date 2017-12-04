#include "q-table.hpp"
#include <iostream>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>

Qtable::Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys,
    std::string const& qtableFilename)
    : stateKeys(stateKeys), actionKeys(actionKeys),
    qtableFilename(qtableFilename)
{
    QValue initial = 0; // initial Q-value

    std::map<int, QValue> actionMap;
    for(auto key : actionKeys){
        actionMap.insert(std::pair<int, QValue>(key, initial));
    }
    std::map<int, QValue>& ref = actionMap;
    for(auto key : stateKeys){
        qValues.insert(std::pair<int,std::map<int, QValue>>(key, ref));
    }
}

Qtable::Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys)
                : Qtable(stateKeys, actionKeys, "") {}

std::ostream& operator<<(std::ostream& os, Qtable const& table) {
    os << "Qtable keys: " << table.getStateKeys()[0] << std::endl
         << table.getActionKeys()[0] << std::endl
         << "Qtable filename: " << table.getQtableFilename();
    return os;
}

QValue& Qtable::getQvalue(int stateKey,
        int actionKey){
    QValue& ref = qValues[stateKey][actionKey];
    return ref;
}
/*

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

int Qtable::getBestMove(int stateIndex){
    int best = 0;
    for (int i = 0; i < getNumberOfMoves(); i++){
        if (getQvalue(stateIndex, i) > getQvalue(stateIndex, best)){
            best = i;
        }
    }
    return best;
}

void Qtable::printQtable(){
    for (int i = 0; i < getNumberOfMoves(); i++){
        for (int j = 0; j < getNumberOfStates(); j++){
            std::cout << std::fixed;
            std::cout << std::setprecision(4) << getQvalue(i, j) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
*/
