  #ifndef FOOTEST_H
  #define FOOTEST_H



//crawler, witch consist of body and two arms.
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
			{       // create 'body'
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
				boxFixtureDef.friction = 1;
  				crawler->CreateFixture(&boxFixtureDef);

				// create arm1
				b2Body* arm1 = NULL;
				myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
  				myBodyDef.position.Set(4, 2); //set the starting position
  				myBodyDef.angle = 0; //set the starting angle
				arm1 = m_world->CreateBody(&myBodyDef);
				
  				boxShape.SetAsBox(1.5,0.1);
  
  				boxFixtureDef.shape = &boxShape;
  				boxFixtureDef.density = 1;
				boxFixtureDef.friction = 10;
  				arm1->CreateFixture(&boxFixtureDef);

                                // create arm2
				b2Body* arm2 = NULL;
				myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
  				myBodyDef.position.Set(7, 2); //set the starting position
  				myBodyDef.angle = 0; //set the starting angle
				
				arm2 = m_world->CreateBody(&myBodyDef);
				
  				boxShape.SetAsBox(1.5,0.1);
  
  				boxFixtureDef.shape = &boxShape;
  				boxFixtureDef.density = 1;
  				arm2->CreateFixture(&boxFixtureDef);

				// set joint crawler-arm1
				b2RevoluteJointDef revoluteJointDef;
 				revoluteJointDef.bodyA = crawler;
				revoluteJointDef.bodyB = arm1;
				revoluteJointDef.collideConnected = false;
				revoluteJointDef.enableMotor = true;
  				revoluteJointDef.maxMotorTorque = 500;
				revoluteJointDef.localAnchorA.Set(3,1);//the top right corner of the box
				revoluteJointDef.localAnchorB.Set(-1.5,0);
  				m_joint = (b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDef );
				
				
				revoluteJointDef.bodyA = arm2;
				revoluteJointDef.bodyB = arm1;
				revoluteJointDef.collideConnected = false;
				revoluteJointDef.localAnchorA.Set(-1.5,0);
				revoluteJointDef.localAnchorB.Set(1.5,0);
  				n_joint = (b2RevoluteJoint*)m_world->CreateJoint( &revoluteJointDef );
				

				m_button = false;
				n_button = false;
			}
   		}
		
		

        void Step(Settings* settings)
        {
            //run the default physics and rendering
            Test::Step(settings); 
    
            //show some text in the main screen
            m_debugDraw.DrawString(5, m_textLine, "Crawler test, w,a,s,d to move joints");
            m_textLine += 15;

        }
    
		void Keyboard(unsigned char key)
		{
			switch (key)
			{
			case 'a':
				m_joint->SetMotorSpeed(2.0f);
				break;

			case 'd':
				m_joint->SetMotorSpeed(-2.0f);
				break;
			
			case 's':
				n_joint->SetMotorSpeed(-2.0f);
				break;

			case 'w':
				n_joint->SetMotorSpeed(2.0f);
				break;
			}
		}

		void KeyboardUp(unsigned char key)
		{
			switch (key)
			{
			case 'a':
				m_joint->SetMotorSpeed(0);
				break;

			case 'd':
				m_joint->SetMotorSpeed(0);
				break;
			
			case 's':
				n_joint->SetMotorSpeed(0);
				break;

			case 'w':
				n_joint->SetMotorSpeed(0);
				break;
			}
		}

        static Test* Create()
        {
            return new TestCrawler;
        }
		b2RevoluteJoint* m_joint;
		b2RevoluteJoint* n_joint;
		bool m_button;
		bool n_button;
    };
  
  #endif
