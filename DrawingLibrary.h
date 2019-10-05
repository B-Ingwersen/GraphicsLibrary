// g++ SDLStreamingExample.cpp -lSDL2 -lpthread -o SDLStreamingExample

#include "SDL2/SDL.h"
#include <iostream>
#include <math.h>
#include <cfloat>
#include <time.h>
#include <unistd.h>

#include <pthread.h>

#define FALSE 0
#define TRUE  1

#define GRAPHICS_MODE_SDL 1
#define GRAPHICS_MODE_FRAMEBUFFER 2

int GRAPHICS_MODE = 0;

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

#include "drawLibHeaders/WindowSections.h"
#include "framebuffer.h"
#define KEY_PRESS 8
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

#include "drawLibHeaders/Keyboard.h"
#include "drawLibHeaders/SizeChange.h"
#include "drawLibHeaders/DrawingFunctions.h"
#include "drawLibHeaders/DrawingStrings.h"
#include "drawLibHeaders/DrawingEvents.h"
#include "drawLibHeaders/DrawingObjects.h"
//#include "SDLAnimation.h"
//#include "SDLAnimationFunctions.h"
#include "drawLibHeaders/DrawingEventHandlers.h"
#include "drawLibHeaders/DrawingFunctionCalls.h"
#include "drawLibHeaders/DrawingFunctionExecute.h"
#include "drawLibHeaders/RenderTimeOps.h"
#include "Extensions/OffscreenBuffers.h"
#include "drawLibHeaders/Mouse.h"
//#include "drawLibHeaders/SDLBasicGUIElements.h"


void * defaultInitializingFunction() {
	cout << "Initilization Finished" << endl;
	return NULL;
}
void* (*initializingFunction)() = defaultInitializingFunction;

int frameCount = 0;
void * drawFramesSDL(void *threadid) {

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

			if (eventsWaiting) {
				SDL_LockSurface(surface);
			}

			processEvents();

			if (eventsWaiting) {
				cout << surface -> pixels << endl;
				SDL_UnlockSurface(surface);
				SDL_UpdateWindowSurface( window );
			}

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
void * drawFramesFRAMEBUFFER(void *threadid) {

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

			/*if (eventsWaiting) {
				SDL_LockSurface(surface);
			}*/
			//paintOverCursor();
			processEvents();

			memcpy( FRAME_BUFFER_MASTER, onScreen -> screen, FB_SCREENSIZE );

			drawCursor();
			/*if (eventsWaiting) {
				SDL_UnlockSurface(surface);
				SDL_UpdateWindowSurface( window );
			}*/

			float timeDiff = float(clock()) - float(lastDrawedFrame);
			if (timeDiff < 14950) { usleep(0.015*CLOCKS_PER_SEC - timeDiff); }

			frameCount++;
		}

		else {
			usleep(15000);
		}
	}

	pthread_exit(NULL);
}

void quitApplication( drawData * callingWindow ) { quit = true; }

void createDrawingEnvironment() {
	float floatMaxTest = FLT_MAX;

	quit = false;
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window * Window = SDL_CreateWindow("BLAHBLAH", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH_PARENT, WINDOW_HEIGHT_PARENT, SDL_WINDOW_RESIZABLE);
	window = Window;

	if (window == NULL) {
		cout << "SDL Initilializing Failed: attempting to switch to Framebuffer mode" << endl;

		getFrameBuffer();
		GRAPHICS_MODE = GRAPHICS_MODE_FRAMEBUFFER;

		int returnValue = system("stty -echo");
		returnValue = system("echo -n -e '\e[?17;14;224c'");

		onScreen = createOffscreenBuffer(WINDOW_WIDTH_PARENT, WINDOW_HEIGHT_PARENT);

		FRAME_BUFFER_SCREEN.screen = (Uint32*)(FRAME_BUFFER_MASTER);
		FRAME_BUFFER_SCREEN.windowWidth = WINDOW_WIDTH_PARENT;
		FRAME_BUFFER_SCREEN.windowHeight = WINDOW_HEIGHT_PARENT;

		mouseX = WINDOW_WIDTH_PARENT / 2;
		mouseY = WINDOW_HEIGHT_PARENT / 2;

	}
	else {
		/*Uint32 rmask = 0x000000ff;
		Uint32 gmask = 0x0000ff00;
		Uint32 bmask = 0x00ff0000;
		Uint32 amask = 0xff000000;*/
		GRAPHICS_MODE = GRAPHICS_MODE_SDL;

		SDL_Surface * Surface = SDL_GetWindowSurface( window );
		surface = Surface;

		Uint32 * Pixels = (Uint32 *)surface->pixels;
		//pixels = Pixels;
		onScreen -> screen = Pixels;
		onScreen -> windowWidth = WINDOW_WIDTH_PARENT;
		onScreen -> windowHeight = WINDOW_HEIGHT_PARENT;

		mouseX = 0;
		mouseY = 0;
		
	}

	//######################################## local code initialization ##################################
	pthread_spin_init(&sizeChangeSpinLock, 0);

	fullWindow = new windowSection;
	(*fullWindow).X1 = 0;
	(*fullWindow).Y1 = 0;
	(*fullWindow).X2 = WINDOW_WIDTH_PARENT - 1;
	(*fullWindow).Y2 = WINDOW_HEIGHT_PARENT - 1;
	fullWindow -> x = 0;
	fullWindow -> y = 0;

	parentWindow = new drawData;
	(*parentWindow).drawFunction = drawWindow;
	(*parentWindow).drawIdentifier = 0;
	(*parentWindow).arguments[0] = 1;
	parentWindow -> arguments[1] = 0;
	parentWindow -> arguments[2] = 0;
	parentWindow -> arguments[3] = WINDOW_WIDTH_PARENT;
	parentWindow -> arguments[4] = WINDOW_HEIGHT_PARENT;
	(*parentWindow).window = fullWindow;

	drawData * parentData = new drawData[128];
	parentData[0].dataPointer = NULL;
	(*parentWindow).dataPointer = parentData;
	(*parentWindow).handlerFunctionsPointer = NULL;

	globalKeyboardHandler = NULL;
	initializeEventHandlers();

	(*initializingFunction)();	// Function that allows program to create the initial window before starting the environment

	//######################################## local code initialization ####################################

	if (GRAPHICS_MODE == GRAPHICS_MODE_SDL) {
		pthread_t * threads = new pthread_t[2];
		pthread_create( &threads[0], NULL, drawFramesSDL, (void *) (intptr_t) 0 );

		lastEvent = 0;
		while (!quit)
		{

			SDL_WaitEvent(&event);

			switch (event.type)
			{
				case SDL_MOUSEMOTION:
				{
					deltaMouseX = event.motion.x - mouseX;
					deltaMouseY = event.motion.y - mouseY;
					mouseX = event.motion.x;
					mouseY = event.motion.y;

					if ( caughtWindow != NULL && leftButtonDown ) {
						callWindowEventHandler( caughtWindow, MOUSE_DRAGGED );
					}
					else {
						if ( eventHandlers[ MOUSE_MOTION ].state ) {
							callEventHandler( MOUSE_MOTION );
						}
						lastMouseWindow = currentMouseWindow;
						currentMouseWindow = findHandlerWindowWithCoords( mouseX, mouseY, parentWindow, WINDOW_ENTER );
						if ( lastMouseWindow != currentMouseWindow ) {
							if ( lastMouseWindow != NULL ) {
								callWindowEventHandler( lastMouseWindow, WINDOW_EXIT );
								//cout << "WINDOW_EXIT\t" << lastMouseWindow << endl;
							}
							if ( currentMouseWindow != NULL) {
								callWindowEventHandler( currentMouseWindow, WINDOW_ENTER );
								//cout << "WINDOW_ENTER\t" << currentMouseWindow << endl;
							}
						}
					}

					if ( sizeChangeWaiting ) {
						addNoOpEvent();
					}

					lastEvent = clock();

					break;
				}
				case SDL_MOUSEBUTTONDOWN:
				{
					mouseX = event.button.x;
					mouseY = event.button.y;

					int mouseButton = event.button.button;

					if ( mouseButton == SDL_BUTTON_LEFT ) {
						lastMouseWindowFocus = mouseWindowFocus;
						mouseWindowFocus = findWindowWithCoords( mouseX, mouseY, parentWindow );

						if ( mouseWindowFocus != lastMouseWindowFocus ) {
							if ( lastMouseWindowFocus != NULL ) {
								callWindowEventHandler( lastMouseWindowFocus, LEAVE_MOUSE_FOCUS );
							}
						}

						leftButtonDown = true;
						if ( eventHandlers[ MOUSE_BUTTON ].state ) {
							callEventHandler( MOUSE_BUTTON );
						}
					}
					else if ( mouseButton == SDL_BUTTON_RIGHT ) {
						rightButtonDown = true;
						if ( eventHandlers[ RIGHT_BUTTON ].state ) {
							callEventHandler( RIGHT_BUTTON );
						}
					}

					lastEvent = clock();

					break;
				}
				case SDL_MOUSEBUTTONUP:
				{
					mouseX = event.button.x;
					mouseY = event.button.y;

					int mouseButton = event.button.button;

					if ( caughtWindow != NULL && leftButtonDown ) {
						callWindowEventHandler( caughtWindow, MOUSE_RELEASE );

						if ( completeOnFinishFunction != NULL ) {
							completeOnFinishFunction( handlingWindowCopy ); // handlingWindowCopy );
						}
						lastEvent = clock();
					}

					if ( mouseButton == SDL_BUTTON_LEFT ) {
						leftButtonDown = false;
						if ( eventHandlers[ MOUSE_RELEASE ].state ) {
							callEventHandler( MOUSE_RELEASE );
						}
					}
					else if ( mouseButton == SDL_BUTTON_RIGHT ) {
						rightButtonDown = false;
					}

					caughtWindow = NULL;

					break;
				}
				case SDL_MOUSEWHEEL:
				{
					scrollX = event.wheel.x;
					scrollY = event.wheel.y;
					scrollTimeOld = scrollTime;
					scrollTime = event.wheel.timestamp;
					
					if ( eventHandlers[ MOUSE_SCROLL ].state ) {
						callEventHandler( MOUSE_SCROLL );
						lastEvent = clock();
					}
					break;
				}
				case SDL_KEYDOWN:
				{
					int keycode = event.key.keysym.sym;
					lastKeyDown = keycode;
					if ( keycode == 1073742049 || keycode == 1073742053 ) {
						SHIFT_DOWN = true;
					}
					else if ( keycode == 1073742048 || keycode == 1073742052 ) {
						CTRL_DOWN = true;
					}
					else if ( keycode == 1073742050 || keycode == 1073742054 ) {
						ALT_DOWN = true;
					}
					else if ( !CTRL_DOWN && !ALT_DOWN ) {
						if ( keyboardWindowFocus != NULL ) {
							callWindowEventHandler( keyboardWindowFocus, KEY_PRESS );
						}
					}
					else if (globalKeyboardHandler != NULL) {
						(*globalKeyboardHandler)( keycode );
					}

					lastEvent = clock();
					break;
				}
				case SDL_KEYUP:
				{
					int keycode = event.key.keysym.sym;
					if ( keycode == 1073742049 || keycode == 1073742053 ) {
						SHIFT_DOWN = false;
					}
					else if ( keycode == 1073742048 || keycode == 1073742052 ) {
						CTRL_DOWN = false;
					}
					else if ( keycode == 1073742050 || keycode == 1073742054 ) {
						ALT_DOWN = false;
					}
					break;
				}
				case SDL_WINDOWEVENT:
				{
					switch (event.window.event) {
						case SDL_WINDOWEVENT_RESIZED: {
							/*WINDOW_WIDTH_PARENT = event.window.data1;
							WINDOW_HEIGHT_PARENT = event.window.data2;
					
							bool wasSizeChangeWaiting = sizeChangeWaiting;
							sizeChangeWaiting = true;*/
							CTRL_DOWN = false; ALT_DOWN = false;
							//eventQueue1Index = 0;
							//eventQueue2Index = 0;

							reconfigureParentWindow( event.window.data1, event.window.data2);
							displayWindow(parentWindow, onScreen);

							break;
						}
						case SDL_WINDOWEVENT_SHOWN: {
							displayWindow( parentWindow, onScreen );
						}
					}
					lastResize = clock();
					break;
				}
				case SDL_QUIT:
				{
					quit = true;
					break;
				}
			}
		}
	}
	else if (GRAPHICS_MODE == GRAPHICS_MODE_FRAMEBUFFER) {
		pthread_t * threads = new pthread_t[2];
		pthread_create( &threads[0], NULL, drawFramesFRAMEBUFFER, (void *) (intptr_t) 0 );

		pthread_create( &threads[1], NULL, processMouse, NULL);
		lastEvent = 0;
		processKeys();

		memset( FRAME_BUFFER_MASTER, 0, FB_SCREENSIZE );
		disconnectFrameBuffer();
		int returnValue = system("stty echo");
	}
}
