#include "interactor.hpp"
#include <iostream>
#include <vector>

Actor::Actor(int ID, std::string const& description,
    int quantizationSteps, float minAngle, float maxAngle,
    std::vector<ActionType> actions)
    : Interactor(ID, description, quantizationSteps, minAngle, maxAngle),
    actions(actions) {}

Sensor::Sensor(int ID, std::string const& description,
    int quantizationSteps, float minAngle, float maxAngle)
    : Interactor(ID, description, quantizationSteps, minAngle, maxAngle) {}

JointActor::JointActor(int ID, int quantizationSteps,
    float minAngle, float maxAngle)
    : Actor(ID, "actor of rotating 2D joint",
    quantizationSteps, minAngle, maxAngle,
    {Still, Counterclockwise, Clockwise}) {}

JointSensor::JointSensor(int ID, int quantizationSteps,
    float minAngle, float maxAngle)
    : Sensor(ID, "sensor of 2D joints angle",
    quantizationSteps, minAngle, maxAngle) {}
