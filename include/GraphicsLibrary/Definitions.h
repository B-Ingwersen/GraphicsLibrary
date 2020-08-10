
#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include <cstdint>
#include "SDL2/SDL.h"

#define FALSE 0
#define TRUE  1

struct screenData {
	uint32_t * screen;
	int windowWidth;
	int windowHeight;
};

struct windowSection {
	int X1;
	int Y1;
	int X2;
	int Y2;
	int x;
	int y;
};

struct drawData {
	void (*drawFunction)(drawData self);
	int drawIdentifier;
	int arguments[16];
	windowSection * window;
	int colorInfo[8];
	void * dataPointer;
	void * handlerFunctionsPointer;
	screenData * screen;
};

typedef void (*functionPointer)(drawData * handlingWindow);

void addNoOpEvent();

extern int WINDOW_HEIGHT_PARENT;
extern int WINDOW_WIDTH_PARENT;

extern clock_t lastEvent, lastDrawedFrame, lastResize;
extern SDL_Surface * surface;
extern SDL_Window * window;
extern SDL_Event event;
extern int mouseX;
extern int mouseY;
extern int scrollX;
extern int scrollY;
extern float scrollTime;
extern float scrollTimeOld;
extern int deltaMouseX;
extern int deltaMouseY;
extern bool quit;
extern bool eventsWaiting;
extern bool sizeChangeWaiting;

extern screenData * onScreen;
extern windowSection * fullWindow;
extern drawData * parentWindow;

extern int rightButtonDown;
extern int leftButtonDown;
extern int middleButtonDown;
extern screenData * cursorBuffer;
extern int cursorBufferX;
extern int cursorBufferY;
extern int oldMouseX;
extern int oldMouseY;

extern drawData * eventWindow;
extern int windowMouseX;
extern int windowMouseY;
extern drawData * caughtWindow;
extern int caughtX;
extern int caughtY;
extern bool windowNotYetMoved;

extern drawData * lastMouseWindow;
extern drawData * currentMouseWindow;
extern drawData * lastMouseWindowFocus;
extern drawData * mouseWindowFocus;

extern functionPointer completeOnFinishFunction;
extern drawData * handlingWindowCopy;

extern void (*prepProcessEvents)();
extern void (*displayProcessedEvents)();
extern void (*eventLoop)();

#endif // _DEFINITION_H_