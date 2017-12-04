#ifndef QLEARNING_9_TABLE_H_
#define QLEARNING_9_TABLE_H_

#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <map>

typedef float QValue;

// This class contains the Q-table and manages the acces to it.
class Qtable {
public:
    Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys,
    std::string const& qtableFilename);

    // Construct Qtable with qtableFilename as "".
    Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys);

    // to do anssi: make this const
    QValue& getQvalue(int stateKey,
                            int actionKey);

    void updateQvalue(const int& stateIndex, const int& actionIndex,
                        QValue& qValue);

    /* Compares actions of a state and returns the largest Q-value.
     */
    QValue getMaxQvalue(int stateIndex);

    /* Generates a random integer (=index)
     * from the range 0 to numberOfMoves-1
     */
    //int getRandomMove(){ return (rand() % numberOfMoves); };

    /* Compares actions
     * and returns the action with the largest Q-value.
     */
    int getBestMove(int stateIndex);

    const std::vector<int>& getStateKeys() const {return stateKeys;};
    const std::vector<int>& getActionKeys() const {return actionKeys;};
    const std::string getQtableFilename() const {return qtableFilename;};

    void printQtable();

    void saveToFile() {
        std::cout << "Dummy save of Qtable." << std::endl;
    };
private:
    // Load Q-Table from the file: qtableFilename.
    void loadFromFile();
    // The QTable is now a vector of vectors (= two dimensional matrix).
    // The size of the matrix doesn't change so the std::vector's ability of
    // size changing is redundant. Is there a better container? Array?

    std::map<int, std::map<int, QValue>> qValues; // qValues[state][action]
    std::vector<int> stateKeys;
    std::vector<int> actionKeys;
    std::string qtableFilename;
};

std::ostream& operator<<(std::ostream& os, Qtable const& table) ;

#endif
