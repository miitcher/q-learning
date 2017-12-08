#include "Render.h"
#include "Test.h"
#include "glui/glui.h"

static void Resize(int32 w, int32 h);
static b2Vec2 ConvertScreenToWorld(int32 x, int32 y);
static void Timer(int);
static void SimulationLoop();
static void Keyboard(unsigned char key, int x, int y);
static void KeyboardSpecial(int key, int x, int y);
static void KeyboardUp(unsigned char key, int x, int y);
static void Mouse(int32 button, int32 state, int32 x, int32 y);
static void MouseMotion(int32 x, int32 y);
static void MouseWheel(int wheel, int direction, int x, int y);
static void Restart(int);
static void Pause(int);
static void Exit(int code);
static void SingleStep(int);
int TestbedMain(int argc, char** argv);
