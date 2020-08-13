
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/RenderInfastructure/SizeChange.h"
#include "pthread.h"
#include <unistd.h>

eventHandler * eventHandlers = new eventHandler[numEventHandlers];
drawData * eventQueue1 = new drawData[16384];
drawData * eventQueue2 = new drawData[16384];
int eventQueue1Index = 0;
int eventQueue2Index = 0;
bool currentEventQueue = 0;
int currentEventFrame = 3141592;
int processEventQueueIndex = 0;
int frameCount = 0;

void addEvent( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, windowSection * window, int numberOfArguments, screenData * screen ) {
	if (currentEventQueue) {
		if ( eventQueue2Index < 16384 ) {
		eventQueue2[eventQueue2Index].drawFunction = drawEventFunction;
		eventQueue2[eventQueue2Index].dataPointer = dataPointer;
		eventQueue2[eventQueue2Index].window = window;
		eventQueue2[eventQueue2Index].drawIdentifier = drawIdentifier;
		eventQueue2[eventQueue2Index].screen = screen;
		int i;
		for (i = 0; i < numberOfArguments; i++) {
			eventQueue2[eventQueue2Index].arguments[i] = arguments[i];
		}
		eventQueue2Index += 1;
		}
	}
	else {
		if ( eventQueue1Index < 16384 ) {
		eventQueue1[eventQueue1Index].drawFunction = drawEventFunction;
		eventQueue1[eventQueue1Index].dataPointer = dataPointer;
		eventQueue1[eventQueue1Index].window = window;
		eventQueue1[eventQueue1Index].drawIdentifier = drawIdentifier;
		eventQueue1[eventQueue1Index].screen = screen;
		int i;
		for (i = 0; i < numberOfArguments; i++) {
			eventQueue1[eventQueue1Index].arguments[i] = arguments[i];
		}
		eventQueue1Index += 1;
		}
	}
}

void addEventExecute( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, windowSection * window, int numberOfArguments, screenData * screen ) {
	drawData command;
	command.drawFunction = drawEventFunction;
	command.dataPointer = dataPointer;
	command.window = window;
	command.drawIdentifier = drawIdentifier;
	command.screen = screen;
	int i;
	for ( i = 0; i < numberOfArguments; i++ ) {
		command.arguments[i] = arguments[i];
	}
	drawEventFunction(command);
}

void addEventNoRepeat( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer,
	windowSection * window, int numberOfArguments, screenData * screen, int * lastDraw) {
	if (currentEventQueue) {
		if (currentEventFrame == lastDraw[0]) {
			int index = lastDraw[1];
			eventQueue2[index].drawFunction = drawEventFunction;
			eventQueue2[index].dataPointer = dataPointer;
			eventQueue2[index].window = window;
			eventQueue2[index].drawIdentifier = drawIdentifier;
			eventQueue2[index].screen = screen;
			int i;
			for (i = 0; i < numberOfArguments; i++) {
				eventQueue2[index].arguments[i] = arguments[i];
			}
		}
		else {
			if ( eventQueue2Index < 16384 ) {
			eventQueue2[eventQueue2Index].drawFunction = drawEventFunction;
			eventQueue2[eventQueue2Index].dataPointer = dataPointer;
			eventQueue2[eventQueue2Index].window = window;
			eventQueue2[eventQueue2Index].drawIdentifier = drawIdentifier;
			eventQueue2[eventQueue2Index].screen = screen;
			int i;
			for (i = 0; i < numberOfArguments; i++) {
				eventQueue2[eventQueue2Index].arguments[i] = arguments[i];
			}
			lastDraw[0] = currentEventFrame;
			lastDraw[1] = eventQueue2Index;
			eventQueue2Index += 1;
			}
		}
	}
	else {
		if (currentEventFrame == lastDraw[0]) {
			int index = lastDraw[1];
			eventQueue1[index].drawFunction = drawEventFunction;
			eventQueue1[index].dataPointer = dataPointer;
			eventQueue1[index].window = window;
			eventQueue1[index].drawIdentifier = drawIdentifier;
			eventQueue1[index].screen = screen;
			int i;
			for (i = 0; i < numberOfArguments; i++) {
				eventQueue1[index].arguments[i] = arguments[i];
			}
		}
		else {
			if ( eventQueue1Index < 16384 ) {
			eventQueue1[eventQueue1Index].drawFunction = drawEventFunction;
			eventQueue1[eventQueue1Index].dataPointer = dataPointer;
			eventQueue1[eventQueue1Index].window = window;
			eventQueue1[eventQueue1Index].drawIdentifier = drawIdentifier;
			eventQueue1[eventQueue1Index].screen = screen;
			int i;
			for (i = 0; i < numberOfArguments; i++) {
				eventQueue1[eventQueue1Index].arguments[i] = arguments[i];
			}
			lastDraw[0] = currentEventFrame;
			lastDraw[1] = eventQueue1Index;
			eventQueue1Index += 1;
			}
		}
	}
}

int getCurrentEventQueueIndex() {
	if (currentEventQueue) {return eventQueue2Index;}
	else {return eventQueue1Index;}
}

void clearEventLogEntries( int firstEvent, int lastEvent ) {
	int i;
	if (currentEventQueue) {
		for ( i = firstEvent; i < lastEvent; i++ ) {
			eventQueue2[eventQueue2Index].drawFunction = NULL;
		}
	}
	if (currentEventQueue) {
		for ( i = firstEvent; i < lastEvent; i++ ) {
			eventQueue1[eventQueue1Index].drawFunction = NULL;
		}
	}
}

void displayWindow( drawData * parentWindow, screenData * screen );

void processEvents() {
		currentEventFrame++;
		
		pthread_spin_lock(&sizeChangeSpinLock);

		if (currentEventQueue) {
			currentEventQueue = 0;
			int i;
			for (i = 0; i < numEventHandlers; i++) {
				eventHandlers[i].hasDrawn = FALSE;
			}
			processEventQueueIndex = 0;
			while (processEventQueueIndex < eventQueue2Index) {
				//(*drawEventFunctions[eventQueue2[i].drawFunction]) (eventQueue2[i]);
				if (eventQueue2[processEventQueueIndex].drawFunction != NULL) {
					eventQueue2[processEventQueueIndex].drawFunction ( eventQueue2[processEventQueueIndex] );
				}
				processEventQueueIndex += 1;
			}
			eventQueue2Index = 0;
		}
		else {
			currentEventQueue = 1;
			int i;
			for (i = 0; i < numEventHandlers; i++) {
				eventHandlers[i].hasDrawn = FALSE;
			}
			processEventQueueIndex = 0;
			while (processEventQueueIndex < eventQueue1Index) {
				if (eventQueue1[processEventQueueIndex].drawFunction != NULL) {
					eventQueue1[processEventQueueIndex].drawFunction ( eventQueue1[processEventQueueIndex] );
				}
				processEventQueueIndex += 1;
			}
			eventQueue1Index = 0;
		}

		pthread_spin_unlock(&sizeChangeSpinLock);
}

void doNothingDrawFunction( drawData drawInfo ) {

}

void addNoOpEvent() {
	addEvent( doNothingDrawFunction , 0, NULL, NULL, NULL, 0, NULL );
}

void * drawFrames(void * threadid) {

	while (!quit) {
		eventsWaiting = FALSE;

		if (currentEventQueue) {
			if (eventQueue2Index) {eventsWaiting = TRUE;}
		}
		else {
			if (eventQueue1Index) {eventsWaiting = TRUE;}
		}

		if (sizeChangeWaiting && (clock() - lastResize) > 1000) { eventsWaiting = true; }
		

		if (eventsWaiting) {
			lastDrawedFrame = clock();

			prepProcessEvents();

			processEvents();

			displayProcessedEvents();

			float timeDiff = float(clock()) - float(lastDrawedFrame);
			if (timeDiff < 14950) { usleep(0.015*CLOCKS_PER_SEC - timeDiff); }

			frameCount++;
		}

		else {
			usleep(15000);
		}
	}

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
	pthread_exit(NULL);
}