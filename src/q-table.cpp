#include "q-table.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>
#include <exception>
#include <fstream>

Qtable::Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys,
    std::string const& qtableFilename)
    : stateKeys(stateKeys), actionKeys(actionKeys),
    qtableFilename(qtableFilename)
{
    if ( stateKeys.size() < 1 || actionKeys.size() < 1){
        throw std::invalid_argument(
                    "Invalid key vector for initialization of Q-table");
    }
// initial value is non-zero because it's used as a factor in the calculations
    QValue initial = 0.1; // initial Q-value of every state-action element

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
    std::cout << "ActionKeys: "<< std::endl;
    for (auto i: table.getActionKeys()){std::cout << i << "; " << std::endl;}
    std::cout << "StateKeys: "<< std::endl;
    for (auto i: table.getStateKeys()){std::cout << i << std::endl;}
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

QValue const& Qtable::getQvalue(int stateKey, int actionKey)  const{

    std::map<int, std::map<int, QValue>>::const_iterator
                                    stateElement  = qValues.find(stateKey);

   // If statekey not found, iterator will be at the end, so throw an exception
    if (qValues.end() == stateElement){

        std::string stri = "getQValue called with an invalid state key: "
                                                                + stateKey;
        throw std::out_of_range(stri);
    }

    std::map<int, QValue>::const_iterator qVal
            = stateElement->second.find(actionKey);

    if (stateElement->second.end() == qVal){
        std::string stri = "getQValue called with an invalid action key: "
                                                                + actionKey;
        throw std::out_of_range(stri);
    }

    return qVal->second;
}

void Qtable::updateQvalue(const int& stateKey, const int& actionKey,
                        QValue& qValue){

    std::map<int, std::map<int, QValue>>::const_iterator
                                    stateElement  = qValues.find(stateKey);

    if (qValues.end() == stateElement){
        throw std::out_of_range(
            "updateQvalue called with an invalid state key");
    }
    std::map<int, QValue>::const_iterator qVal
            = stateElement->second.find(actionKey);

    if (stateElement->second.end() == qVal){
        throw std::out_of_range(
            "updateQvalue called with an invalid action key");
    }

    for (auto action: qValues[stateKey]){
        if (action.first == actionKey){
            qValues[stateKey][actionKey] = qValue;
        }
    }
}

QValue const& Qtable::getMaxQvalue(int const& stateKey) const{
    try{
        QValue max = getQvalue(stateKey, getActionKeys()[0]);
        for (auto it : getActionKeys()){
            if (getQvalue(stateKey, it) > max){
                max = getQvalue(stateKey, it);
            }
        }
        QValue const& ref = max;
        return ref;
    }catch(std::exception& e){
        std::cerr << "exception caught in getMaxQvalue: " << e.what() << '\n';
        QValue const& ref = 0;
        return ref;
    }
}

int Qtable::getBestAction(int const& stateKey) const{
    try{
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
    }catch(std::exception& e){
        std::cerr << "exception caught in getBestAction: " << e.what() << '\n';
        QValue const& ref = 0;
        return ref;
    }
}

void Qtable::saveToFile()
{
    std::ofstream file(qtableFilename, std::ios_base::binary);
    file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    try {
        std::cout << "saving to file: " << qtableFilename << std::endl;
        for(auto state : stateKeys){
            for (auto action : actionKeys){
                QValue value = qValues[state][action];
                file.write( (char *)& value, sizeof(QValue));
            }
        }
        file.close();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Could not save to file";
    }
}

void Qtable::loadFromFile(){
    std::ifstream file(qtableFilename, std::ios_base::binary);
    file.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    QValue value;
    try {
        std::cout << "loading from file: " << qtableFilename << std::endl;
        for(auto state : stateKeys){
            for (auto action : actionKeys){
                file.read((char *)& value, sizeof(QValue));
                //std::cout   << "Reading value from file: "
                  //          << std::fixed << value << std::endl;
                qValues[state][action] = value;
            }
        }
        file.close();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Could not load from file";
    }
}
