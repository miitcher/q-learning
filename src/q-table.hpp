#ifndef QLEARNING_9_TABLE_H_
#define QLEARNING_9_TABLE_H_

#include "interactor.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <map>

#define FRIEND_TEST(test_case_name, test_name)\
friend class test_case_name##_##test_name##_Test

typedef float QValue;

// This class contains the Q-table and manages the acces to it.
class Qtable {
public:
    Qtable() {}; // Default constructor

    Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys,
    std::string const& qtableFilename);

    // Construct Qtable with qtableFilename as "".
    Qtable(std::vector<int> stateKeys, std::vector<int> actionKeys);

    QValue const& getQvalue(int stateKey, int actionKey) const;

    void updateQvalue(const int& stateKey, const int& actionKey,
                        QValue& qValue);

    /* Compares actions of a state and returns the largest Q-value.
     */
    QValue const& getMaxQvalue(int const& stateKey)const;

    /* Compares actions
     * and returns the actionKey with the largest Q-value.
     */
    int getBestAction (int const& stateIndex) const;

    const std::vector<int>& getStateKeys() const {return stateKeys;};
    const std::vector<int>& getActionKeys() const {return actionKeys;};
    const std::string& getQtableFilename() const {return qtableFilename;};

    // Save Q-table to the given filename.
    void saveToFile(std::string filename);
    // Generate new unique filename for Qtable.qtableFilename,
    // and save Q-table with that filename.
    void saveToFile();
    // Load Q-table from the given filename.
    void loadFromFile(std::string filename);
    // Load Q-table from the filename: Qtable.qtableFilename.
    void loadFromFile();
private:
    FRIEND_TEST(test_Qtable, test_save_and_load);

    std::map<int, std::map<int, QValue>> qValues; // qValues[state][action]
    std::vector<int> stateKeys;
    std::vector<int> actionKeys;
    std::string qtableFilename;
};

std::ostream& operator<<(std::ostream& os, Qtable const& table) ;

#endif
