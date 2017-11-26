#ifndef LEARNING_9
#define LEARNING_9

#include <vector>
#include <string>

typedef float QvalueType;
typedef int StateType;
typedef int ActionType;
typedef int RewardType;
typedef double SensorInputType;
typedef std::pair<int, ActionType> ActionPacketType;        // (actorID, Action)
typedef std::pair<int, SensorInputType> ResponsePacketType; // (sensorID, SensorInput)

// TODO: Mikael
// An agent has actors. The actor can do things in the simulation,
// depending on the actor type; e.g. a actor can be a joint, that can rotate
// in 2D-space.
class Actor {
public:
    Actor();
    const int& getActorID();
    const std::string& getDescription();

    int returnTwo(); // TODO: remove
private:
    int actorID;
    std::string description;
};

// TODO: Mikael
// An agent has sensors. The sensor can sense things in the
// simulation, depending on the sensor type; e.g. a angle-sensor can detect
// in what angle a joint is, for the agent.
class Sensor {
public:
    Sensor();
    const int& getActorID();
    const std::string& getDescription();
private:
    int sensorID;
    std::string description;
};

// TODO: Anssi
// Vector of state-vectors that contain actions for that state.
class Qtable {
public:
    Qtable(std::vector<Actor> actors, std::vector<Sensor> sensors);

    QvalueType& getQvalue(StateType state, ActionType action);

    ActionType& getBestAction(StateType state);


    ActionType& getRandomAction(StateType state);

    // Compares actions and returns the largest Q-value
    QvalueType& getOptimalFutureQvalue(StateType state);

    void updateQvalue(StateType state, ActionType action, QvalueType qValue);

private:
    std::vector<std::vector<QvalueType>> qValues; // qValues[state][action]
};

// TODO: Anssi
class Agent {
public:
    Agent(std::vector<Actor> actors, std::vector<Sensor> sensors, Qtable qtable);

    // Chooses the best or a random action
    ActionType& chooseAction(StateType state);

    void updateQtable(StateType state, ActionType action, StateType nextState);

    StateType getState();

    // The action tells what the agent tries to do in the simulation.
    void doAction(std::vector<ActionPacketType> actionMessage);

    // The response is the result of an action in the simulation.
    RewardType& calcReward(std::vector<ResponsePacketType> responseMessage);

private:
    int ID;
    double discountFactor;      // range 0...1, e.g. 0.9, increase
    double learningRate;        // range: 0...1, e.g. 0.1
    double explorationFactor;   // range: 0...1, e.g. 0.5, decrease
    StateType currentState;

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
};

// TODO: Mikael
// Initializes and controlls the threads where agents and their simulation is done.
class AgentManager {
public:
    // Function used by threads to run the learning and simulation of one agent
    void agentThreadFunction(); /* TODO: add parameter for agent and simulation
    classes, and possibly saved Q-values*/

    // Reads files and creates threads that contain an agent and its simulation
    void initRun(std::string simulationFileName, std::string qvaluesFilename);

    // Has the fittest agent teach the other agents, when multiple agents
    // are learning. Copies the Q-values of the fittest agent to other agents
    // from the previous eneration and modifies the parameters in the
    // Q-learning function (discountFactor, etc.).
    void evolveAgents();

    // Pauses all the threads
    void pauseSimulation();

    // Makes the first agent save its Q-values to file
    void saveQvalues();

    // Terminate threads
    void endSimulation();

private:
    // TODO: Find out how to handle threads
    // std::vector<thread_type> agentThreads;
};

#endif
