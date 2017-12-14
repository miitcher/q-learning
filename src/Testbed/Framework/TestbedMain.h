#include "Render.h"
#include "Test.h"



//resizes window if it is manipulated by user
void Resize(int32 w, int32 h);

//converts received pixel sized window to metres to allow proper scaling of box2d
b2Vec2 ConvertScreenToWorld(int32 x, int32 y);

// This is used to control the frame rate (60Hz).
void Timer(int);

//simulationloop starter, actual simulationloop is in test.h/c
void SimulationLoop();

//assign special keys to zoom, reset etc
void Keyboard(unsigned char key, int x, int y);

//function that defines moving viewable area via arrow keys
void KeyboardSpecial(int key, int x, int y);

//funtion to handle when key is up
void KeyboardUp(unsigned char key, int x, int y);

//Mouse is used move objects in world
void Mouse(int32 button, int32 state, int32 x, int32 y);

//Mouse is used to move world around, rightclick
void MouseMotion(int32 x, int32 y);

//Zoom view with mouse
void MouseWheel(int wheel, int direction, int x, int y);

//deletes current world and starts new
void Restart(int);

//pauses simulation
void Pause(int);

//exits simulation when shut down
void Exit(int code);

//Pauses and runs single step after witch is paused again
void SingleStep(int);


int TestbedMain(int argc, char** argv);
