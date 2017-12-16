#ifndef CRAWLERTEST_H
#define CRAWLERTEST_H




#include <unistd.h>

//crawler, which consists of body and two arms.
//floor is also made in this file



  class TestCrawler : public Test
    {
        public:
        TestCrawler()
		{
			//Create floor
			b2Body* ground = NULL;
			{
				b2BodyDef bd;
				ground = m_world->CreateBody(&bd);

				b2EdgeShape shape;
				shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
				ground->CreateFixture(&shape, 0.0f);
			}

			//Create crawler
			b2Body* crawler = NULL;
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


				//debugtests
				//crawler->SetTransform(b2Vec2(10,20),1);
				//b2Vec2 crawlerposition = crawler->GetPosition();
				//m_debugDraw.DrawString(5, m_textLine, "Position:%.3f,%.3f",crawlerposition.x,crawlerposition.y);
				//m_textLine += 15;

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

				shoulderJointDef.lowerAngle = -1;
				shoulderJointDef.upperAngle = 0.5;
				shoulderJointDef.maxMotorTorque = 500;

				//the top right corner of the body of crawler
				shoulderJointDef.localAnchorA.Set(3,1);
				shoulderJointDef.localAnchorB.Set(-1.5,0);//at left end of upperarm
				shoulder = (b2RevoluteJoint*)m_world->CreateJoint(
												&shoulderJointDef );


				//create elbow joint and set its properties
				b2RevoluteJointDef elbowJointDef;
				elbowJointDef.bodyA = forearm;
				elbowJointDef.bodyB = upperArm;

				elbowJointDef.collideConnected = false;
				elbowJointDef.enableMotor = true;
				elbowJointDef.enableLimit = true;

				elbowJointDef.lowerAngle = -0.5;
				elbowJointDef.upperAngle = 2.5;
				elbowJointDef.maxMotorTorque = 500;
				elbowJointDef.localAnchorA.Set(-1.5,0);     //left end of forearm
				elbowJointDef.localAnchorB.Set(1.5,0);      //right end of upperarm
				elbow = (b2RevoluteJoint*)m_world->CreateJoint( &elbowJointDef );

			}
   		}

		void rotateArm(float speed)
		{
			shoulder->SetMotorSpeed(speed);
		}

		void rotateForearm(float speed)
		{
			elbow->SetMotorSpeed(speed);
		}


        void Step(Settings* settings)
        {

			//run the default physics and rendering
			Test::Step(settings);

			//show some text in the main screen
			m_debugDraw.DrawString(5, m_textLine, "Crawler test, w,a,s,d to move joints. t to location and crash");
			m_textLine += 15;

			//display location of body for debug, causes segmentation fault
			//crawlerposition = new b2Vec2;
			//float rcawlerposition.x = crawler->GetPosition().x;
			//void crawler->SetTransform(b2Vec2(10,20),1);

			//Angle of joints in RAD
			float elbowangle = elbow->GetJointAngle();
			float shoulderangle = shoulder->GetJointAngle();
			const b2Vec2& crawlerposition = crawler->GetWorldCenter();
			//const float mass = crawler->GetMass();

			//Display angles
			m_debugDraw.DrawString(5, m_textLine, "Elbow angle: %.2f RAD",float(elbowangle));
			m_textLine += 15;
			m_debugDraw.DrawString(5, m_textLine, "Shoulder angle: %.2f RAD",float(shoulderangle));
			m_textLine += 15;
			m_debugDraw.DrawString(5, m_textLine, "Position:%.3f,%.3f",&crawlerposition.x,&crawlerposition.y);
			m_textLine += 15;


        }

		void Keyboard(unsigned char key)
		{
			switch (key)
			{
			case 'a':
				rotateArm(2.0f);
				break;

			case 'd':
				rotateArm(-2.0f);
				break;

			case 's':
				rotateForearm(-2.0f);
				break;

			case 'w':
				rotateForearm(2.0f);
				break;

			}
		}

		void KeyboardUp(unsigned char key)
		{
			switch (key)
			{
			case 'a':
				rotateArm(0);
				break;

			case 'd':
				rotateArm(0);
				break;

			case 's':
				rotateForearm(0);
				break;

			case 'w':
				rotateForearm(0);
				break;
			}
		}

	//Return object  of crawler to testbed
        static Test* Create()
        {
            return new TestCrawler;
        }

	//Stored references to bodies
		b2Body* forearm;
		b2Body* upperArm;
		b2Body* crawler;
		b2RevoluteJoint* shoulder;
		b2RevoluteJoint* elbow;
    };

  #endif
