#ifndef QLEARNING_9_TABLE_H_
#define QLEARNING_9_TABLE_H_

#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>

typedef float QValue;

// This class contains the Q-table and manages the acces to it.
class Qtable {
public:

    Qtable(int s, int a);

    const QValue& getQvalue(const int& stateIndex,
        const int& actionIndex) const;

    void updateQvalue(const int& stateIndex, const int& actionIndex,
                        QValue& qValue);

    /* Compares actions of a state and returns the largest Q-value.
     */
    QValue getMaxQvalue(int stateIndex);

    int getRandomAction(){ return (rand() % numberOfActions); };

    /* Compares actions
     * and returns the action with the largest Q-value.
     */
    int getBestAction(int stateIndex);

    const int& getNumberOfStates() const {return numberOfStates;};
    const int& getNumberOfActions() const {return numberOfActions;};

    void printQtable();

    // read and write
    void saveToFile();
    void loadFromFile(const std::string& qtableFilename);

private:
    std::vector<std::vector<QValue>> qValues; // qValues[state][action]
    int numberOfStates;
    int numberOfActions;

};

#endif
