#include "GraphicsLibrary/Core.h"
#include "SDLFramework/Framework.h"
#include "FBFramework/Framework.h"
#include <iostream>

void quitApplication( drawData * callingWindow ) { quit = true; }

void createDrawingEnvironment( void (*initializingFunction)(void * argument), void * initializingArgument ) {
	quit = false;

	bool initSuccess = SDLFramework_Initialize();
	if (!initSuccess) {
		initSuccess = FBFramework_Initialize();
	}
	if (!initSuccess) {
		std::cout << "Error: could not find a suitable framework. Exiting now" << std::endl;
		exit(0);
	}

	//######################################## local code initialization ##################################
	pthread_spin_init(&sizeChangeSpinLock, 0);

	fullWindow = new windowSection;
	fullWindow -> X1 = 0;
	fullWindow -> Y1 = 0;
	fullWindow -> X2 = WINDOW_WIDTH_PARENT - 1;
	fullWindow -> Y2 = WINDOW_HEIGHT_PARENT - 1;
	fullWindow -> x = 0;
	fullWindow -> y = 0;

	parentWindow = new drawData;
	parentWindow -> drawFunction = drawWindow;
	parentWindow -> drawIdentifier = 0;
	parentWindow -> arguments[0] = 1;
	parentWindow -> arguments[1] = 0;
	parentWindow -> arguments[2] = 0;
	parentWindow -> arguments[3] = WINDOW_WIDTH_PARENT;
	parentWindow -> arguments[4] = WINDOW_HEIGHT_PARENT;
	parentWindow -> window = fullWindow;

	drawData * parentData = new drawData[128];
	parentData[0].dataPointer = NULL;
	parentWindow -> dataPointer = parentData;
	parentWindow -> handlerFunctionsPointer = NULL;

	globalKeyboardHandler = NULL;
	initializeEventHandlers();

	initializingFunction(initializingArgument);	// Function that allows program to create the initial window before starting the environment

	//######################################## local code initialization ####################################

	pthread_t * thread = new pthread_t;
	pthread_create( thread, NULL, drawFrames, (void *) (intptr_t) 0 );
	eventLoop();
}