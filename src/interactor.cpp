#include "interactor.hpp"
#include <iostream>
#include <vector>
#include <sstream>

std::ostream& operator<<(std::ostream& os, Interactor const& p) {
    os << "Interactor( ID: " << p.getID() << ", '" << p.getDescription()
        << "', \t{" << p.getQuantizationSteps() << ", "
        << p.getMinAngle() << ", " << p.getMaxAngle() << "} )";
    return os;
}

Actor::Actor(int ID, std::string const& description,
    int quantizationSteps, float minAngle, float maxAngle,
    std::vector<Action> actions)
    : Interactor(ID, description, quantizationSteps, minAngle, maxAngle),
    actions(actions) {}

Sensor::Sensor(int ID, std::string const& description,
    int quantizationSteps, float minAngle, float maxAngle)
    : Interactor(ID, description, quantizationSteps, minAngle, maxAngle) {}

JointActor::JointActor(int ID, int quantizationSteps,
    float minAngle, float maxAngle)
    : Actor(ID, "Actor of rotating 2D joint.",
    quantizationSteps, minAngle, maxAngle,
    {Still, Counterclockwise, Clockwise}) {}

JointSensor::JointSensor(int ID, int quantizationSteps,
    float minAngle, float maxAngle)
    : Sensor(ID, "Sensor of 2D joints angle.",
    quantizationSteps, minAngle, maxAngle) {}

XAxisSensor::XAxisSensor(int ID)
    : Sensor(ID, "Sensor of Agents position on the x-axis.", 0, 0, 0) {}
