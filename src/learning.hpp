#ifndef LEARNING_9
#define LEARNING_9

#include <vector>
#include <string>
#include <thread>

typedef float QvalueType;
typedef int StateType;
typedef int RewardType;
typedef double SensorInputType;
typedef int AgentShapeType;
// TODO: agentShape will be implemented according to the needs of the simulation.

/**
Possible moves for an Actor. The enumerators are integers, so
they can be used as keys in the Q-table.
If other actions are possible, then they need to be added to the enum.
*/
enum ActionType {
    Still,
    Counterclockwise,
    Clockwise
};

typedef std::pair<int, ActionType> ActionPacketType;
// (actorID, Action)
typedef std::pair<int, SensorInputType> ResponsePacketType;
// (sensorID, SensorInput)

/**
An interactor is the parent class for actor or a sensor. An agent can
interact on the world with actors, and sense the world with sensors.
For now we only consider interactors that "use" angles (joints), but
other interactors can be added.

When it is an actor it is a joint that can rotate in 2D space.
When it is a sensor it can sense what angle the sensor is in 2D space.
*/
class Interactor {
public:
    Interactor(int ID, std::string const& description, int quantizationSteps,
        float minAngle, float maxAngle)
        : ID(ID), description(description),
        quantizationSteps(quantizationSteps),
        minAngle(minAngle), maxAngle(maxAngle) {}
    // access data
    const int& getID() const { return ID; }
    const std::string& getDescription() const { return description; }
    const int& getQuantizationSteps() const { return quantizationSteps; }
    const float& getMinAngle() const { return minAngle; }
    const float& getMaxAngle() const { return maxAngle; }
private:
    int ID;
    std::string description;
    // When looking at an angle, then the angle between two quantization
    // steps nearest eachother is the smallest angle used.
    // The quantizationSteps tells how many states an sensor can be in,
    // if the interactor is a sensor.
    int quantizationSteps; // 2 ^ X
    // The min and max angles limit the angle, if angles are used.
    // An angle is in reference to the unit circle.
    // Therefore an angle of 0 is to the right, an angle of pi/2 is up and
    // the max angle is counterclockwise of the min angle.
    // If the angles are the same, then the angle is not limited.
    float minAngle;
    float maxAngle;
};

class Actor : public Interactor {
public:
    Actor(int ID, std::string const& description,
        int quantizationSteps, float minAngle, float maxAngle,
        std::vector<ActionType> actions);
    const std::vector<ActionType>& getActions() const { return actions; }
    int getNumberOfActions() const { return actions.size(); }
private:
    std::vector<ActionType> actions;
};

class Sensor : public Interactor {
public:
    Sensor(int ID, std::string const& description,
        int quantizationSteps, float minAngle, float maxAngle);
};

// The acting part of a 2D joint.
class JointActor : public Actor {
public:
    JointActor(int ID, int quantizationSteps, float minAngle, float maxAngle);
};

// The sensing part of a 2D joint.
class JointSensor : public Sensor {
public:
    JointSensor(int ID, int quantizationSteps,
        float minAngle, float maxAngle);
};

// TODO: Anssi
// Vector of state-vectors that contain actions for that state.
class Qtable {
public:
    Qtable(std::vector<Actor> interactors, std::vector<Sensor> sensors);

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
// Initializes and controlls the threads where agents and their simulation are.
class AgentManager {
public:
    AgentManager(std::vector<Actor>& actors, std::vector<Sensor>& sensors,
        AgentShapeType& agentShape, unsigned int agentCount);

    // Function used by threads to run the learning and simulation of one agent
    void agentThreadTask();
    // TODO: add parameter for agent and simulation
    // classes, and possibly saved Q-values

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
    std::vector<std::thread> agentThreads;
};

/* TODO: Mikael - THOUGHTS ABOUT THREADING (connected to AgentManager threads)
Use mutex on q-table when copying it between generations.
The mutex locks the resours. The mutex would be in the copy function of the q-table.

The copying of a q-table from one thread to many threads could be done
by giving the other threads a reference or pointer to the q-table

When a agent gets to the goal, all agents will stop moving and then some signal/event happens
that tells all the agents to move to the beginning (change state in
learning and in simulation) and copying the q-table.


Alternatively all but the winer would survive between generations, and
the other new Agents for the next generation would be created from scratch.
Thus the new Agents Q-table would be made by the copy constructor.

Could always create one Agent, and then copy it. This would be done
in the beginning of the run and between every genereation.
With only one Agent the only difference would be not copying and
that there is no goal.

condition_variable could be used to have the thread communicate with each other.


Is the method of discarding all but the winners Q-values effective computatively?
*/

#endif
