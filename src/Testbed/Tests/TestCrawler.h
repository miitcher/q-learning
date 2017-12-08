  #ifndef FOOTEST_H
  #define FOOTEST_H

#include <unistd.h>


//crawler, which consists of body and two arms.
//floor is also made in this file


    
  class TestCrawler : public Test
    {
        public:
        TestCrawler() 
		{
			b2Body* ground = NULL;
			{
				b2BodyDef bd;
				ground = m_world->CreateBody(&bd);

				b2EdgeShape shape;
				shape.Set(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
				ground->CreateFixture(&shape, 0.0f);
			}

			b2Body* crawler = NULL;
			{       
				// create 'body'
				b2BodyDef myBodyDef;
	  			myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	  			myBodyDef.position.Set(0, 1); //set the starting position
	  			myBodyDef.angle = 0; //set the starting angle
				crawler = m_world->CreateBody(&myBodyDef);
				
				b2PolygonShape boxShape;
	  			boxShape.SetAsBox(3,1);
	  
	  			b2FixtureDef boxFixtureDef;
	  			boxFixtureDef.shape = &boxShape;
	  			boxFixtureDef.density = 1;
				boxFixtureDef.friction = 0.5f;
  				crawler->CreateFixture(&boxFixtureDef);

				// create arm1
				b2Body* upperArm = NULL;
				myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
	  			myBodyDef.position.Set(4, 2); //set the starting position
	  			myBodyDef.angle = 0; //set the starting angle
				upperArm = m_world->CreateBody(&myBodyDef);
				
	  			boxShape.SetAsBox(1.5,0.1);
	  
	  			boxFixtureDef.shape = &boxShape;
	  			boxFixtureDef.density = 1;
				boxFixtureDef.friction = 100;
  				upperArm->CreateFixture(&boxFixtureDef);

                                // create arm2
				b2Body* forearm = NULL;
				myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
  				myBodyDef.position.Set(7, 2); //set the starting position
  				myBodyDef.angle = 0; //set the starting angle
				
				forearm = m_world->CreateBody(&myBodyDef);
				
	  			boxShape.SetAsBox(1.5,0.1);
	  
	  			boxFixtureDef.shape = &boxShape;
	  			boxFixtureDef.density = 1;
	  			forearm->CreateFixture(&boxFixtureDef);

				// set joint to arms
				b2RevoluteJointDef revoluteJointDef;
 				revoluteJointDef.bodyA = crawler;
				revoluteJointDef.bodyB = upperArm;
				revoluteJointDef.collideConnected = false;
				revoluteJointDef.enableMotor = true;
  				revoluteJointDef.maxMotorTorque = 500;
				revoluteJointDef.localAnchorA.Set(3,1);  //the top right corner of the box
				revoluteJointDef.localAnchorB.Set(-1.5,0);  //
				shoulder = (b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDef );
				
				
				revoluteJointDef.bodyA = forearm;
				revoluteJointDef.bodyB = upperArm;
				revoluteJointDef.collideConnected = false;
				revoluteJointDef.localAnchorA.Set(-1.5,0);
				revoluteJointDef.localAnchorB.Set(1.5,0);
  				elbow = (b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDef );
				
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
            m_debugDraw.DrawString(5, m_textLine, "Crawler test, w,a,s,d to move joints");
            m_textLine += 15;
						
						//display angle of joints and location of body for debug
						//b2Vec2 pos = forearm->GetPosition();

						//Angle of joints in RAD
            float elbowangle = elbow->GetJointAngle();
						float shoulderangle = shoulder->GetJointAngle();

						m_debugDraw.DrawString(5, m_textLine, "Elbow angle: %.2f RAD",float(elbowangle));
            m_textLine += 15;
						m_debugDraw.DrawString(5, m_textLine, "Shoulder angle: %.2f RAD",float(shoulderangle));
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

        static Test* Create()
        {
            return new TestCrawler;
        }
		b2Body* crawler;
		b2RevoluteJoint* shoulder;
		b2RevoluteJoint* elbow;
    };
  
  #endif
