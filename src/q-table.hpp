#ifndef QLEARNING_9_TABLE_H_
#define QLEARNING_9_TABLE_H_

#include "learning.hpp"
#include <vector>
#include <string>
#include <iostream>

// This class contains the Q-table and manages the acces to it.
class Qtable {
public:
    /* The Qtable is initialized from actors and sensors. Initializer needs the
     * the number each actor's possible actions and the number of each
     * state-detecting sensor's possible states. Qtable is in a vector of
     * state-vectors that contain actions for that state. The size of the Qtable
     * will be states * actions. If there is for example three kinds of actors:
     * actors with 3, 2 and 2 possible actions, for each state there is
     * 3 * 2 * 2 = 12 possible actions. If three state-detecting sensors can
     * detect each 10 different states, the total number of states is
     * 10³ = 1000. This would make a Qtable of a size 1000 * 12. The "ID" of
     * each action and state is the index in the vector.
     */
    Qtable(std::vector<Actor> const& actors, std::vector<Sensor> const& sensors);

    QvalueType getQvalue(StateType const& state, ActionType const& action);

    void updateQvalue(const StateType& state, const ActionType& action,
                        QvalueType& qValue);

    ActionType& getRandomAction(const StateType& state);

    /* Compares actions of a state and returns the largest Q-value.
     */
    QvalueType& getMaxQvalue(const StateType& state);

    /* Compares actions
     * and returns the action with the largest Q-value.
     */
    ActionType& getBestAction(const StateType& state);

    int getNumberOfStates(){return numberOfStates;};
    int getNumberOfActions(){return numberOfActions;};

private:
    std::vector<std::vector<QvalueType>> qValues; // qValues[state][action]
    int numberOfStates;
    int numberOfActions;

};

#endif
