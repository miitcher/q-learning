#ifndef QLEARNING_9_INTERACTOR_H_
#define QLEARNING_9_INTERACTOR_H_

#include <vector>
#include <string>
#include <thread>
#include <ostream>

/**
Possible moves for an Actor. The enumerators are integers, so
they can be used as keys in the Q-table.
If other actions are possible, then they need to be added to the enum.
*/
enum Move {
    Still,
    Counterclockwise,
    Clockwise
};

// SensorInput is unquantisized.
typedef double SensorInput;
typedef std::pair<int, Move> ActorAction;
    // (actorID, Move)
// ResponsePacket is unquantisized.
typedef std::pair<int, SensorInput> ResponsePacket;
    // (sensorID, SensorInput)
typedef std::vector<ActorAction> Action;
// State is unquantisized.
typedef std::vector<ResponsePacket> State;

/**
An interactor is the parent class for the actor and the sensor. An agent can
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
    // The angles are in radians.
    // The min and max angles limit the angle, if angles are used.
    // An angle is in reference to the unit circle.
    // Therefore an angle of 0 rad is to the right, an angle of pi/2 rad is up
    // and the max angle is counterclockwise of the min angle.
    // If the angles are the same, then the angle is not limited.
    float minAngle;
    float maxAngle;
};

std::ostream& operator<<(std::ostream& os, Interactor const& p);

class Actor : public Interactor {
public:
    Actor(int ID, std::string const& description,
        int quantizationSteps, float minAngle, float maxAngle,
        std::vector<Move> actions);
    const std::vector<Move>& getMoves() const { return actions; }
    int getNumberOfMoves() const { return actions.size(); }
private:
    std::vector<Move> actions;
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

/* Sensor that senses the AgentLearners position in the x-axis. This can be used
for calculating the revard when the goal is to travel to the right.
*/
class XAxisSensor : public Sensor {
public:
    XAxisSensor(int ID);
};

#endif
