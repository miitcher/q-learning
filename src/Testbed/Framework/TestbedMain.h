#include "Render.h"
#include "Test.h"

void Resize(int32 w, int32 h);
b2Vec2 ConvertScreenToWorld(int32 x, int32 y);
void Timer(int);
void SimulationLoop();
void Keyboard(unsigned char key, int x, int y);
void KeyboardSpecial(int key, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void Mouse(int32 button, int32 state, int32 x, int32 y);
void MouseMotion(int32 x, int32 y);
void MouseWheel(int wheel, int direction, int x, int y);
void Restart(int);
void Pause(int);
void Exit(int code);
void SingleStep(int);
int TestbedMain(int argc, char** argv);
