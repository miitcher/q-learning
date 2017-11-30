  #ifndef FOOTEST_H
  #define FOOTEST_H
    
  class Testv : public Test
    {
        public:
        Testv() { } //do nothing, no scene yet
    
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
