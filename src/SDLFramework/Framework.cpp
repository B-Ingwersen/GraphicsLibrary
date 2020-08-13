
#include "SDL2/SDL.h"
#include "SizeChange.h"
#include "Keyboard.h"
#include "GraphicsLibrary/Definitions.h"
#include "GraphicsLibrary/Inputs/Keyboard.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEventHandlers.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/RenderInfastructure/WindowSections.h"
#include <time.h>

void SDLFramework_prepProcessEvents() {
    SDL_LockSurface(surface);
}
void SDLFramework_displayProcessedEvents() {
    SDL_UnlockSurface(surface);
    SDL_UpdateWindowSurface( window );
}

void SDLFramework_EventLoop();

bool SDLFramework_Initialize() {
    SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window * Window = SDL_CreateWindow("Graphics Library Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH_PARENT, WINDOW_HEIGHT_PARENT, SDL_WINDOW_RESIZABLE);
	window = Window;

    if (window == NULL) {
        return false;
    }

    SDL_Surface * Surface = SDL_GetWindowSurface( window );
    surface = Surface;

    Uint32 * Pixels = (Uint32 *)surface->pixels;
    onScreen -> screen = Pixels;
    onScreen -> windowWidth = WINDOW_WIDTH_PARENT;
    onScreen -> windowHeight = WINDOW_HEIGHT_PARENT;

    prepProcessEvents = SDLFramework_prepProcessEvents;
    displayProcessedEvents = SDLFramework_displayProcessedEvents;
    eventLoop = SDLFramework_EventLoop;

    mouseX = 0;
    mouseY = 0;
    return true;
}

void SDLFramework_EventLoop() {
    handleKeyDown = handleKeyDownSDL;

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
                        }
                        if ( currentMouseWindow != NULL) {
                            callWindowEventHandler( currentMouseWindow, WINDOW_ENTER );
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
                lastKeyUp = keycode;
                if ( keycode == 1073742049 || keycode == 1073742053 ) {
                    SHIFT_DOWN = false;
                }
                else if ( keycode == 1073742048 || keycode == 1073742052 ) {
                    CTRL_DOWN = false;
                }
                else if ( keycode == 1073742050 || keycode == 1073742054 ) {
                    ALT_DOWN = false;
                }

                if ( keyboardWindowFocus != NULL ) {
                    callWindowEventHandler( keyboardWindowFocus, KEY_RELEASE );
                }

                break;
            }
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED: {
                        CTRL_DOWN = false; ALT_DOWN = false;

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