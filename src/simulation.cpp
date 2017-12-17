#include "simulation.hpp"
#include "interactor.hpp"
#include <vector>
#include <stdexcept>
#include <Box2D/Box2D.h>
#include <stdio.h>
#include <thread>
#include <iostream>
#include <sstream>
#include "Testbed/Framework/TestbedMain.h"


Simulation::Simulation(unsigned& agentID,
    std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    drawGraphics(drawGraphics)
{
    // At the moment, this class is combatible only with crawlers that have
    // two joints. The one connected to the body is called shoulder and the
    // other is called elbow.

    // Shoulder of the crawler has ID 1 and elbow has ID 2
    shoulderID = 1;
    elbowID =    2;
    bool foundShoulder = false;
    bool foundElbow = false;
    for (auto actor : actors){
        if (actor.getID() == 1){
            shoulderMinAngle = actor.getMinAngle();
            shoulderMaxAngle = actor.getMaxAngle();
            foundShoulder = true;
        }
        else if(actor.getID() == 2){
            elbowMinAngle = actor.getMinAngle();
            elbowMaxAngle = actor.getMaxAngle();
            foundElbow = true;
        }
    }
    if (!foundShoulder){
        throw std::invalid_argument(
    "Shoulder actor of the crawler was not found. Shoulder should have ID 1.");
    }
    if (!foundElbow){
        throw std::invalid_argument(
    "Elbow actor of the crawler was not found. Elbow should have ID 2.");
    }


    //create world with gravity
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    m_world = new b2World(gravity);
    //std::cout << "world created, ";

    //Create floor
    ground = NULL;
    {
        b2BodyDef grounddef;
        grounddef.position.Set(0.0f, 0.0f);
        ground = m_world->CreateBody(&grounddef);
        b2EdgeShape shape;
        shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
        ground->CreateFixture(&shape, 0.0f);
        //b2Vec2 gposition = ground->GetPosition();
        //std::cout   << "ground created, groundpos x:" << gposition.x << " y:"
        //            << gposition.y << std::endl;
    }

    crawler = NULL;
    forearm = NULL;
    upperarm = NULL;
    {
        // create main body of crawler
        b2BodyDef myBodyDef;
        myBodyDef.type = b2_dynamicBody;  //this will be a dynamic body
        myBodyDef.position.Set(0, 1);     //set the starting position
        myBodyDef.angle = 0;              //set the starting angle
        crawler = m_world->CreateBody(&myBodyDef);
        b2PolygonShape boxShape;
        boxShape.SetAsBox(3,1);
        b2FixtureDef boxFixtureDef;
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 1;
        boxFixtureDef.friction = 0.5f;
        crawler->CreateFixture(&boxFixtureDef);

        // create upperarm
        b2Body* upperArm = NULL;
        myBodyDef.type = b2_dynamicBody;   //this will be a dynamic body
        myBodyDef.position.Set(4, 2);      //set the starting position
        myBodyDef.angle = 0;               //set the starting angle
        upperArm = m_world->CreateBody(&myBodyDef);
        boxShape.SetAsBox(1.5,0.1);
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 1;
        boxFixtureDef.friction = 100;
        upperArm->CreateFixture(&boxFixtureDef);

        // create forearm
        b2Body* forearm = NULL;
        myBodyDef.type = b2_dynamicBody;  //this will be a dynamic body
        myBodyDef.position.Set(7, 2);     //set the starting position
        myBodyDef.angle = 0;              //set the starting angle
        forearm = m_world->CreateBody(&myBodyDef);
        boxShape.SetAsBox(1.5,0.1);
        boxFixtureDef.shape = &boxShape;
        boxFixtureDef.density = 1;
        forearm->CreateFixture(&boxFixtureDef);

        //create shoulder joint and set its properties
        b2RevoluteJointDef shoulderJointDef;
        shoulderJointDef.bodyA = crawler;
        shoulderJointDef.bodyB = upperArm;
        shoulderJointDef.collideConnected = false;
        shoulderJointDef.enableMotor = true;
        shoulderJointDef.enableLimit = true;

        // the angle of the joints go beyond the lowerAngle and upperAngle,
        // which is suboptimal.
        shoulderJointDef.lowerAngle = shoulderMinAngle + 0.2;
        shoulderJointDef.upperAngle = shoulderMaxAngle - 0.3;
        shoulderJointDef.maxMotorTorque = 500;
        //the top right corner of the body of crawler
        shoulderJointDef.localAnchorA.Set(3,1);
        shoulderJointDef.localAnchorB.Set(-1.5,0);  //at left end of upperarm
        shoulder = (b2RevoluteJoint*)m_world->CreateJoint( &shoulderJointDef );

        //create elbow joint and set its properties
        b2RevoluteJointDef elbowJointDef;
        elbowJointDef.bodyA = forearm;
        elbowJointDef.bodyB = upperArm;
        elbowJointDef.collideConnected = false;
        elbowJointDef.enableMotor = true;
        elbowJointDef.enableLimit = true;
        elbowJointDef.lowerAngle = elbowMinAngle + 0.2;
        elbowJointDef.upperAngle = elbowMaxAngle - 0.3;
        elbowJointDef.maxMotorTorque = 500;
        elbowJointDef.localAnchorA.Set(-1.5,0);     //left end of forearm
        elbowJointDef.localAnchorB.Set(1.5,0);      //right end of upperarm
        elbow = (b2RevoluteJoint*)m_world->CreateJoint( &elbowJointDef );
    }
}

State Simulation::moveAgentToBeginning() {
    // The crawler is initialized in the correct position in the constructor,
    // so this function does not change the position at the moment.

    // Angles of joints in radians
    float elbowangle = elbow->GetJointAngle();
    SensorInput convertedElbow = static_cast<SensorInput>(elbowangle);

    float shoulderangle = shoulder->GetJointAngle();
    SensorInput convertedShoulder = static_cast<SensorInput>(shoulderangle);

    // Position on x-axis
    float crawlerLocation = crawler->GetPosition().x;
    SensorInput convertedLocation = static_cast<SensorInput>(crawlerLocation);

    // create a state to return to the learning classes
    ResponsePacket responsePacket0(999, convertedLocation);
    ResponsePacket responsePacket1(shoulderID, convertedShoulder);
    ResponsePacket responsePacket2(elbowID, convertedElbow);
    return {responsePacket0, responsePacket1, responsePacket2};
}

State Simulation::simulateAction(Action& action) {

    for(auto actorAction : action){
        switch (actorAction.second)
        {
        case Still:
            break;

        case Counterclockwise:
            if( actorAction.first == shoulderID){ // shoulder
                shoulder->SetMotorSpeed(2.0f);
                break;
            } else if (actorAction.first == elbowID){ // elbow
                elbow->SetMotorSpeed(2.0f);
                break;
            }
        case Clockwise:
            if( actorAction.first == shoulderID){ // shoulder
                shoulder->SetMotorSpeed(-2.0f);
                break;
            } else if (actorAction.first == elbowID){ // elbow
                elbow->SetMotorSpeed(-2.0f);
                break;
            }
        }
    }

    float timeStep = 1.0f / 60.0f;
    int velocityIterations = 6;
    int positionIterations = 2;
    // This function simulates the physics for some time.
    // If timeStep is increased, joints go out of their range!!!!!!!!!!!!
    // Which is suboptimal.
    m_world->Step(timeStep, velocityIterations, positionIterations);

    // Angles of joints in radians
    float elbowangle = elbow->GetJointAngle();
    SensorInput convertedElbow = static_cast<SensorInput>(elbowangle);
    float shoulderangle = shoulder->GetJointAngle();
    SensorInput convertedShoulder = static_cast<SensorInput>(shoulderangle);

    // Position on x-axis
    float crawlerLocation = crawler->GetPosition().x;
    SensorInput convertedLocation = static_cast<SensorInput>(crawlerLocation);

/*  // debug
    std::cout   << "location:      " << convertedLocation << std::endl
                << "elbowangle:    " << elbowangle << std::endl
                << "shoulderangle: " << shoulderangle << std::endl;
*/

    // create a state to return to the AgentLearner receiveSimulationResponse-
    // function
    ResponsePacket responsePacket0(999, convertedLocation);
    ResponsePacket responsePacket1(shoulderID, convertedShoulder);
    ResponsePacket responsePacket2(elbowID, convertedElbow);
    return {responsePacket0, responsePacket1, responsePacket2};
}
