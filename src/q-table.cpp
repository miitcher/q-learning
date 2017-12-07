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
    QValue initial = 0; // initial Q-value of every state-action element

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
    os << "Qtable state dimension: " << table.getStateKeys().size()
        << std::endl
        << "Qtable action dimension: " << table.getActionKeys().size()
        << std::endl
        << "Qtable filename: " << table.getQtableFilename()
        << std::endl;

        // debug
        for(auto state : table.getStateKeys()){
            for(auto action : table.getActionKeys()){
                os << std::fixed;
                os << std::setprecision(4);
                os << table.getQvalue(state, action) << " ";
            }
            os << std::endl;
        }
        os << std::endl;
    return os;
}

QValue const& Qtable::getQvalue(int stateKey,
        int actionKey) const {
    // flower_bound finds the first key that is not less than the given.
    QValue const& ref = qValues.lower_bound(stateKey)->second.
                                lower_bound(actionKey)->second;
    return ref;
}

int Qtable::updateQvalue(const int& stateKey, const int& actionKey,
                        QValue& qValue){
    // returns now 1 when stateKey exists and 0 when it doesn't -> make better?
    for (auto action: qValues[stateKey]){
        if (action.first == actionKey){
            qValues[stateKey][actionKey] = qValue;
            return 1;
        }
    }
    return 0;
}

QValue Qtable::getMaxQvalue(int stateKey){
    QValue empty = -1;
    size_t one = 1;
    if (qValues[stateKey].size() < one) return empty;
              //returns -1 from empty --> to do Anssi: make better

    QValue max = getQvalue(stateKey, getActionKeys()[0]);

    for (auto it : qValues[stateKey]){
        if (it.second > max){
            max = it.second;
        }
    }
    return max;
}

int Qtable::getBestAction(int stateKey){
    QValue empty = -1;
    size_t one = 1;
    if (qValues[stateKey].size() < one) return empty;
              //returns -1 from empty --> to do Anssi: make better

    // initializes values as the first action
    int bestActionKey = getActionKeys()[0];
    QValue max = getQvalue(stateKey, bestActionKey);

    for (auto it : getActionKeys()){
        if (getQvalue(stateKey, it) > max){
            bestActionKey = it;
            max = getQvalue(stateKey, it);
        }
    }
    return bestActionKey;
}
