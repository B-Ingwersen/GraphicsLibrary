
#ifndef _RENDER_INFASTRUCTURE___DRAWING_EVENTS_H_
#define _RENDER_INFASTRUCTURE___DRAWING_EVENTS_H_

#define numEventHandlers 12

#include "GraphicsLibrary/Definitions.h"

struct eventHandler {
	int state;
	void (*globalHandler)();
	bool hasDrawn;
	int firstEvent;
	int lastEvent;
};

extern eventHandler * eventHandlers;
extern drawData * eventQueue1;
extern drawData * eventQueue2;
extern int eventQueue1Index;
extern int eventQueue2Index;
extern bool currentEventQueue;
extern int currentEventFrame;
extern int processEventQueueIndex;
extern int frameCount;

void addEvent( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, windowSection * window, int numberOfArguments, screenData * screen );

void addEventExecute( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, windowSection * window, int numberOfArguments, screenData * screen );

void addEventNoRepeat( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer,
	windowSection * window, int numberOfArguments, screenData * screen, int * lastDraw);

int getCurrentEventQueueIndex();

void clearEventLogEntries( int firstEvent, int lastEvent );

void displayWindow( drawData * parentWindow, screenData * screen );

void processEvents();

void doNothingDrawFunction( drawData drawInfo );

void addNoOpEvent();

void * drawFrames(void * threadid);

#endif // _RENDER_INFASTRUCTURE___DRAWING_EVENTS_H_