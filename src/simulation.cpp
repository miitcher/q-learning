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

//NEED to iclude required framework for box2d
//HEAVILY unfinished, lots to do!

void helloThere() {
    std::cout << "Simulation: hello" << std::endl;
}

// TODO: This should be done better inside the Simulation-class.
void runTestbed(unsigned agentID) {
    std::string agentID_string = std::to_string(agentID);
    std::cout << "agentID_string: " << agentID_string << std::endl;
    // Max 100 agent-threads.
    if (agentID_string.size() > 2) {
        throw std::runtime_error("Too many agent-threads.");
    }
    char he[3] = {agentID_string[0], agentID_string[1], '\0'};

    //char* argv[] = {"Hello"};
    char* argv[] = {he};
    TestbedMain(1, argv);
}

Simulation::Simulation(unsigned& agentID,
    std::vector<Actor>& actors, std::vector<Sensor>& sensors,
    AgentShape& agentShape, bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    drawGraphics(drawGraphics)
{

    shoulderID =          actors[0].getID();
    elbowID =             actors[1].getID();
    shoulderMinAngle =    actors[0].getMinAngle();
    elbowMinAngle =       actors[1].getMinAngle();
    shoulderMaxAngle =    actors[0].getMaxAngle();
    elbowMaxAngle =       actors[1].getMaxAngle();


    //create world with gravity
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    b2World* m_world = new b2World(gravity);
    std::cout << "world created, ";

    //Create floor
    b2Body* ground = NULL;
    {
        b2BodyDef grounddef;
        grounddef.position.Set(0.0f, 0.0f);
        ground = m_world->CreateBody(&grounddef);
        b2EdgeShape shape;
        shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
        ground->CreateFixture(&shape, 0.0f);
        b2Vec2 gposition = ground->GetPosition();
        std::cout   << "ground created, groundpos x:" << gposition.x << " y:"
                    << gposition.y << std::endl;
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
        shoulderJointDef.lowerAngle = shoulderMinAngle;
        shoulderJointDef.upperAngle = shoulderMaxAngle;
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
        elbowJointDef.lowerAngle = elbowMinAngle;
        elbowJointDef.upperAngle = elbowMaxAngle;
        elbowJointDef.maxMotorTorque = 500;
        elbowJointDef.localAnchorA.Set(-1.5,0);     //left end of forearm
        elbowJointDef.localAnchorB.Set(1.5,0);      //right end of upperarm
        elbow = (b2RevoluteJoint*)m_world->CreateJoint( &elbowJointDef );
    }
}

State Simulation::moveAgentToBeginning() {

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
/*
b2Body* forearm;
b2Body* upperArm;
b2Body* crawler;

*/
/*
// USE 4 SPACES INSTEAD OF TABS!

//get current position of crawler joints and location
ResponsePacket getcrawlerstate(int responseid){
    float elbowangle = elbow->GetJointAngle();
    float shoulderangle = shoulder->GetJointAngle();
    float crawlerposition = 0; //TODO get crawler position working
    //actorid 1 = shoulder, 2 = elbow, 999 = position(x-coordinate);
    if(responseid == 1){
    ResponsePacket shoulderresponce(1,shoulderangle);
    return shoulderresponce;
    }
    if(responseid == 2){
    ResponsePacket elbowresponce(2,elbowangle);
    return elbowresponce;
    }
    if(responseid == 999){
    ResponsePacket locationresponce(999,crawlerposition);
    return locationresponce;
    }
    else{
    //returns empty responsepacket if called wrong responceid
    return ResponcePacket(0,0);
    }
}
*/

