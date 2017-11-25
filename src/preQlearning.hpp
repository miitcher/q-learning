functions for Q-learner

//Mikael
class Actor(){

}
class Sensor(){

}

typedef float QvalueType;
typedef int stateType;
typedef int actionType;
typedef int rewardType;
typedef double sensorInputType;
// add typedef for action and responses

//anssi
class Qtable(std::vector<Actor> actors, std::vector<Sensor> sensors){
    /* vector of state-vectors that contain actions for that state
    */
public:
    Qtable()
    /* qValues is initialized from actors and sensors */

    QvalueType& getQvalue(stateType state, actionType action){
    /*
    */
    return qValues[state][action];


    actionType& getBestAction(stateType state);


    actionType& getRandomAction(stateType state);


    qValueType& getOptimalFutureQvalue(stateType state);
    /* Compares actions
     * and returns the largest Q-value
     */

    void updateQvalue(stateType state, actionType action, qValueType qValue);


private:
    std::vector<std::vector<QvalueType>> qValues; // qValues[state][action]

}

//anssi

class Agent(std::vector<Actor> actors, std::vector<Sensor> sensors, Qtable qtable)
{
public:
    actionType& chooseAction(state);
    // chooses the best or a random action


    rewardType& calcReward(std::vector<std::pair<int, sensorInputType>> response);
    // response from simulation

    void updateQtable(stateType state, actionType action, stateType nextState);

    void doAction(std::vector<std::pair<int, actionType>> action);
    //action to simulation

private:
    double discountFactor; // range 0...1, e.g. 0.9, increase
    double learningRate; // range: 0...1, e.g. 0.1
    double explorationFactor; // range: 0...1, e.g. 0.5, decrease

    int state;

}

// Mikael

class AgentManager(){
public:
    agentThreadFunction(&Qvalues, ) //all parameter for agent and simulation classes
    /* The function used by thread to run the learning and simulation of one
       agent
     */

    initRun(simulationFileName, qvaluesFilename){
    // reads files and creates threads that contain an agent and a simulation
    }

    evolveAgents{
    /* Copies the qValues of the best agent to other agents from the previous
     * generation and modifies the parameters in the Q-learning function.
     */
    }

    pauseSimulation()
    // pauses all the threads

    saveQvalues()
    // makes the first agent save its Q-values to file

    endSimulation()
    // terminate threads

private:
    std::vector<thread_type> agentThreads;
}
