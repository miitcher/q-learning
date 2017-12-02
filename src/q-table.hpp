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
    Qtable(int stateDimension, int actionDimension,
        std::string const& qtableFilename);

    // Construct Qtable with qtableFilename as "".
    Qtable(int stateDimension, int actionDimension);

    const QValue& getQvalue(const int& stateIndex,
        const int& actionIndex) const;

    void updateQvalue(const int& stateIndex, const int& actionIndex,
                        QValue& qValue);

    /* Compares actions of a state and returns the largest Q-value.
     */
    QValue getMaxQvalue(int stateIndex);

    /* Generates a random integer (=index)
     * from the range 0 to numberOfActions-1
     */
    int getRandomAction(){ return (rand() % numberOfActions); };

    /* Compares actions
     * and returns the action with the largest Q-value.
     */
    int getBestAction(int stateIndex);

    const int& getNumberOfStates() const {return numberOfStates;};
    const int& getNumberOfActions() const {return numberOfActions;};

    void printQtable();

    void saveToFile();
private:
    // Load Q-Table from the file: qtableFilename.
    void loadFromFile();
    // The QTable is now a vector of vectors (= two dimensional matrix).
    // The size of the matrix doesn't change so the std::vector's ability of
    // size changing is redundant. Is there a better container? Array?

    std::vector<std::vector<QValue>> qValues; // qValues[state][action]
    int numberOfStates;
    int numberOfActions;
    std::string qtableFilename;
};

#endif
