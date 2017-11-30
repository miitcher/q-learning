  #ifndef FOOTEST_H
  #define FOOTEST_H
    
  class Testv : public Test
    {
        public:
        Testv() 
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
				b2BodyDef myBodyDef;
  				myBodyDef.type = b2_dynamicBody; //this will be a dynamic body
  				myBodyDef.position.Set(0, 1); //set the starting position
  				myBodyDef.angle = 0; //set the starting angle
				crawler = m_world->CreateBody(&myBodyDef);
				
				b2PolygonShape boxShape;
  				boxShape.SetAsBox(2,1);
  
  				b2FixtureDef boxFixtureDef;
  				boxFixtureDef.shape = &boxShape;
  				boxFixtureDef.density = 1;
  				crawler->CreateFixture(&boxFixtureDef);
			}
   		}
		
        void Step(Settings* settings)
        {
            //run the default physics and rendering
            Test::Step(settings); 
    
            //show some text in the main screen
            m_debugDraw.DrawString(5, m_textLine, "Now we have a foo test");
            m_textLine += 15;
        }
    
        static Test* Create()
        {
            return new Testv;
        }
    };
  
  #endif
