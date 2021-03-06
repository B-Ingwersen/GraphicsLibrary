
#include "SizeChange.h"
#include "GraphicsLibrary/Definitions.h"
#include "GraphicsLibrary/RenderInfastructure/SizeChange.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEventHandlers.h"
#include "GraphicsLibrary/Inputs/Keyboard.h"
#include <pthread.h>

void reconfigureParentWindow(int width, int height) {
	pthread_spin_lock(&sizeChangeSpinLock);

	WINDOW_WIDTH_PARENT = width;
	WINDOW_HEIGHT_PARENT = height;

	int arg5 = parentWindow -> arguments[5];
	int arg6 = parentWindow -> arguments[6];
	int arg7 = parentWindow -> arguments[7];
	int arg8 = parentWindow -> arguments[8];
	parentWindow -> arguments[5] = parentWindow -> arguments[1];
	parentWindow -> arguments[6] = parentWindow -> arguments[2];
	parentWindow -> arguments[7] = parentWindow -> arguments[3];
	parentWindow -> arguments[8] = parentWindow -> arguments[4];

	onScreen -> windowWidth = width;
	onScreen -> windowHeight = height;

    surface = SDL_GetWindowSurface( window );
    onScreen -> screen = (Uint32 *)(surface->pixels);
    
	ALT_DOWN = false;
	CTRL_DOWN = false;
	leftButtonDown = false;
	caughtWindow = NULL;
	lastMouseWindow = NULL;
	currentMouseWindow = NULL;
	lastMouseWindowFocus = NULL;
	mouseWindowFocus = NULL;

	parentWindow -> arguments[3] = width;
	parentWindow -> arguments[4] = height;
	parentWindow -> window -> X2 = width - 1;
	parentWindow -> window -> Y2 = height - 1;

	drawData * windowData = (drawData*)(parentWindow -> dataPointer);
	int i;
	int maxIndex = parentWindow -> arguments[0];
	for ( i = 1; i < maxIndex; i++ ) {
		callWindowEventHandler( (drawData*)(parentWindow -> dataPointer) + i, 10 );
		if ( i == 126 ) { windowData = (drawData *)(windowData[127].dataPointer); i = 0; maxIndex -= 126; }
	}

	parentWindow -> arguments[5] = arg5;
	parentWindow -> arguments[6] = arg6;
	parentWindow -> arguments[7] = arg7;
	parentWindow -> arguments[8] = arg8;

	if ( sizeChangeFunction != NULL ) { (*sizeChangeFunction)(); }

	pthread_spin_unlock(&sizeChangeSpinLock);
}