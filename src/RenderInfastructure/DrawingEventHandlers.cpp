
#include "GraphicsLibrary/RenderInfastructure/DrawingEventHandlers.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/RenderInfastructure/WindowSections.h"

void doNothingEventHandler() {}

void coverEventHandler( drawData * window ) {}

void initializeEventHandlers() {
	int i;
	for ( i = 0; i < numEventHandlers; i++ ) {
		eventHandlers[i].state = HANDLER_DISABLED;
		eventHandlers[i].globalHandler = doNothingEventHandler;
		eventHandlers[i].hasDrawn = FALSE;
	}
}

void setHandlerFunction( int handlerID, void newHandler() ) {
	if (handlerID < numEventHandlers and handlerID >= 0) {
		eventHandlers[ handlerID ].globalHandler = newHandler;
		if ( eventHandlers[ handlerID ].state == HANDLER_DISABLED ) {
			eventHandlers[ handlerID ].state = GLOBAL_HANDLER;
		}
	}
}

void setHandlerState( int handlerID, int newHandlerState ) {
	eventHandlers[ handlerID ].state = newHandlerState;
}

void disableHandler( int handlerID ) {
	eventHandlers[ handlerID ].state = HANDLER_DISABLED;
}

void callGlobalEventHandler( int handlerID ) {
	if (eventHandlers[ handlerID ].hasDrawn) {
		int firstEvent = eventHandlers[ handlerID ].firstEvent;
		int lastEvent = eventHandlers[ handlerID ].lastEvent;

		eventHandlers[ handlerID ].firstEvent = getCurrentEventQueueIndex();
		eventHandlers[ handlerID ].globalHandler();
		eventHandlers[ handlerID ].lastEvent = getCurrentEventQueueIndex();
		
		clearEventLogEntries( firstEvent, lastEvent );
	}
	else {
		eventHandlers[ handlerID ].hasDrawn = TRUE;

		eventHandlers[ handlerID ].firstEvent = getCurrentEventQueueIndex();
		eventHandlers[ handlerID ].globalHandler();
		eventHandlers[ handlerID ].lastEvent = getCurrentEventQueueIndex();
	}
}

void clearWindowEvents() {
	lastMouseWindow = NULL;
	currentMouseWindow = NULL;
}

void callWindowEventHandler( drawData * handlingWindow, int handlerID ) {

	completeOnFinishFunction = NULL;			// copies this data so that may be completed even if some data is deleted
	handlingWindowCopy = handlingWindow;			// saves window as an argument for the function

	if ( handlingWindow -> handlerFunctionsPointer != NULL ) {	// checks if there are handling functions present
		functionPointer * handlingFunctions = (functionPointer *) ((*handlingWindow).handlerFunctionsPointer);
		eventWindow = handlingWindow;
		windowMouseX = mouseX - (*(*handlingWindow).window).X1;
		windowMouseY = mouseY - (*(*handlingWindow).window).Y1;
		
		if ( handlingFunctions[handlerID] != NULL ) {
			handlingFunctions[handlerID]( handlingWindow ); //( handlingWindow );
		}
	}
	else if (handlingWindow -> drawIdentifier == 0) {
		drawData * parentWindow = getParentWindow( handlingWindow );
		if (parentWindow != NULL) {
			callWindowEventHandler( parentWindow, handlerID );
		}
		/*else if ( eventHandlers[ handlerID ].state != HANDLER_DISABLED ) {
			callGlobalEventHandler( handlerID );
		}*/
	}
}

void callEventHandler( int handlerID ) {
	if (eventHandlers[ handlerID ].state == GLOBAL_HANDLER) {
		callGlobalEventHandler( handlerID );
	}
	else if (eventHandlers[ handlerID ].state == MOUSE_LOCATION_HANDLER) {
		drawData * mouseWindow = findHandlerWindowWithCoords( mouseX, mouseY, parentWindow, handlerID);
		//cout << mouseWindow << endl;
		callWindowEventHandler( mouseWindow, handlerID );
	}
}

void addWindowEventHandler( drawData * handlingWindow, int handlerID, void handlerFunction( drawData * handlingWindow ) ) {
	if ( (*handlingWindow).handlerFunctionsPointer == NULL ) {
		functionPointer * handlingFunctions = new functionPointer[numEventHandlers];
		int i;
		for ( i = 0; i < numEventHandlers; i++ ) {
			handlingFunctions[i] = NULL; 
		}
		(*handlingWindow).handlerFunctionsPointer = handlingFunctions;
	}
	functionPointer * handlingFunctions =  (functionPointer *) ((*handlingWindow).handlerFunctionsPointer);
	handlingFunctions[handlerID] = handlerFunction;
}

void deleteWindowEventHandler( drawData * handlingWindow, int handlerID ) {
	if ( (*handlingWindow).handlerFunctionsPointer != NULL ) {
		functionPointer * handlingFunctions = (functionPointer *) ((*handlingWindow).handlerFunctionsPointer);
		handlingFunctions[handlerID] = NULL;
	}
}
















