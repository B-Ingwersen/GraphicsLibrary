int WINDOW_HEIGHT_PARENT = 900;
int WINDOW_WIDTH_PARENT = 1600;

using namespace std;

clock_t lastEvent, lastDrawedFrame, lastResize;
SDL_Surface * surface;
SDL_Window * window;
SDL_Event event;
int mouseX;
int mouseY;
int scrollX;
int scrollY;
float scrollTime;
float scrollTimeOld;
int deltaMouseX;
int deltaMouseY;
bool quit;
bool eventsWaiting;
bool sizeChangeWaiting = false;

screenData * onScreen = new screenData;
windowSection * fullWindow;
drawData * parentWindow;

void addNoOpEvent();
int rightButtonDown = false;
int leftButtonDown = false;
int middleButtonDown = false;
screenData * cursorBuffer = NULL;
int cursorBufferX;
int cursorBufferY;
int oldMouseX;
int oldMouseY;

drawData * eventWindow;
int windowMouseX;
int windowMouseY;
drawData * caughtWindow = NULL;
int caughtX;
int caughtY;
bool windowNotYetMoved;

drawData * lastMouseWindow = NULL;
drawData * currentMouseWindow = NULL;
drawData * lastMouseWindowFocus = NULL;
drawData * mouseWindowFocus = NULL;

functionPointer completeOnFinishFunction = NULL;
drawData * handlingWindowCopy = NULL;

void (*prepProcessEvents)();
void (*displayProcessedEvents)();
void (*eventLoop)();