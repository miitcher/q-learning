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
        // for(auto state : table.getStateKeys()){
        //     for(auto action : table.getActionKeys()){
        //         os << std::fixed;
        //         os << std::setprecision(4);
        //         os << table.getQvalue(state, action) << " ";
        //     }
        //     os << std::endl;
        // }
        // os << std::endl;
    return os;
}

QValue const& Qtable::getQvalue(int stateKey,
        int actionKey) const {
    // find doesn't discard const qualifiers
    QValue const& ref = qValues.find(stateKey)->second.
                                find(actionKey)->second;
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

QValue const& Qtable::getMaxQvalue(int const& stateKey) const{
    QValue max = getQvalue(stateKey, getActionKeys()[0]);

    for (auto it : getActionKeys()){
        if (getQvalue(stateKey, it) > max){
            max = getQvalue(stateKey, it);
        }
    }
    QValue const& ref = max;
    return ref;
}

int Qtable::getBestAction(int const& stateKey) const{
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
