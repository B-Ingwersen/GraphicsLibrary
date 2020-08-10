
#include "Mouse.h"
#include "Framebuffer.h"
#include "GraphicsLibrary/RenderInfastructure/WindowSections.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEventHandlers.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/Drawing/OffscreenBuffers.h"
#include "GraphicsLibrary/Drawing/Functions.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

void * processMouse(void * threadid) {
    int fd;
	char path[] = "/dev/input/mice";
    fd = open(path, O_RDONLY);
    char data[3];

    int returnValue = system("stty -echo");

    int bytes, left, right, middle, x, y;

    while (!quit)
    {
        bytes = read(fd, &data, 3 * sizeof(char));

        if (bytes > 0) {
            left = data[0] & 0x1;
            right = (data[0] & 0x2) >> 1;
            middle = (data[0] & 0x4) >> 2;

            deltaMouseX = data[1];
            deltaMouseY = -data[2];

            bool move = false;


            if (deltaMouseX != 0) { move = true; }
            if (deltaMouseY != 0) { move = true; }

            if (cursorBuffer == NULL) {
                cursorBuffer = createOffscreenBuffer( 20, 20 );
            }
            oldMouseX = mouseX;
            oldMouseY = mouseY;
            if (!middle) {
                mouseX += deltaMouseX;
                mouseY += deltaMouseY;
            }
            if (mouseX < 0) { mouseX = 0;} if (mouseX >= WINDOW_WIDTH_PARENT) { mouseX = WINDOW_WIDTH_PARENT - 1;}
            if (mouseY < 0) { mouseY = 0;} if (mouseY >= WINDOW_HEIGHT_PARENT) { mouseY = WINDOW_HEIGHT_PARENT - 1;}

            if (leftButtonDown != left) {
                leftButtonDown = left;
                if (leftButtonDown) {
                    lastMouseWindowFocus = mouseWindowFocus;
					mouseWindowFocus = findWindowWithCoords( mouseX, mouseY, parentWindow );

					if ( mouseWindowFocus != lastMouseWindowFocus ) {
						if ( lastMouseWindowFocus != NULL ) {
							callWindowEventHandler( lastMouseWindowFocus, LEAVE_MOUSE_FOCUS );
						}
					}
					if ( eventHandlers[ MOUSE_BUTTON ].state ) {
						callEventHandler( MOUSE_BUTTON );
					}
                }
                else {
                    if ( caughtWindow != NULL ) {
                        callWindowEventHandler( caughtWindow, MOUSE_RELEASE );

                        if ( completeOnFinishFunction != NULL ) {
                            completeOnFinishFunction( handlingWindowCopy ); // handlingWindowCopy );
                        }
                        lastEvent = clock();
                    }
                    if ( eventHandlers[ MOUSE_RELEASE ].state ) {
                        callEventHandler( MOUSE_RELEASE );
                    }
                }
            }
            if (rightButtonDown != right) {
                rightButtonDown = right;
                if (rightButtonDown) {
					if ( eventHandlers[ RIGHT_BUTTON ].state ) {
						callEventHandler( RIGHT_BUTTON );
					}
                }
            }
            if (middleButtonDown != middle) {
                middleButtonDown = middle;
            }

            if (move) {
                addNoOpEvent();

                if ( middleButtonDown ) {
                    scrollX = deltaMouseX;
                    scrollY = -deltaMouseY;
                    //scrollTimeOld = scrollTime;
                    //scrollTime = event.wheel.timestamp;
                    
                    if ( eventHandlers[ MOUSE_SCROLL ].state ) {
                        callEventHandler( MOUSE_SCROLL );
                        //lastEvent = clock();
                    }
                    
                    continue;
                }

				if ( caughtWindow != NULL && (leftButtonDown || rightButtonDown) ) {
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
            }
        }
        //cout << x << "\t" << y << "\t" << left << "\t" << middle << "\t" << right << endl;
    }

    pthread_exit(NULL);
}

void drawCursor() {
    drawData command;
    cursorBufferX = mouseX - 10;
    cursorBufferY = mouseY - 10;
    /*if (cursorBuffer == NULL) {
        cursorBuffer = createOffscreenBuffer( 20, 20 );
    }

    command.arguments[0] = cursorBufferX;
    command.arguments[1] = cursorBufferY;
    command.arguments[2] = 0;
    command.arguments[3] = 0;
    command.arguments[4] = 20;
    command.arguments[5] = 20;

    command.dataPointer = &FRAME_BUFFER_SCREEN;
    command.screen = cursorBuffer;

    copyBufferSection(command);*/

    command.screen = &FRAME_BUFFER_SCREEN;
    command.window = parentWindow -> window;

    command.arguments[0] = cursorBufferX + 8 ;
    command.arguments[1] = cursorBufferY;
    command.arguments[2] = 4;
    command.arguments[3] = 8;
    command.arguments[4] = 0xFF0000;
    drawRectangle(command);
    command.arguments[0] = cursorBufferX;
    command.arguments[1] = cursorBufferY + 8;
    command.arguments[2] = 8;
    command.arguments[3] = 4;
    command.arguments[4] = 0xFF0000;
    drawRectangle(command);
    command.arguments[0] = cursorBufferX + 12;
    command.arguments[1] = cursorBufferY + 8;
    command.arguments[2] = 8;
    command.arguments[3] = 4;
    command.arguments[4] = 0xFF0000;
    drawRectangle(command);
    command.arguments[0] = cursorBufferX + 8;
    command.arguments[1] = cursorBufferY + 12;
    command.arguments[2] = 4;
    command.arguments[3] = 8;
    command.arguments[4] = 0xFF0000;
    drawRectangle(command);

    //cout << "blah" << endl;
}
void paintOverCursor() {
    if (cursorBuffer == NULL) {
        cursorBuffer = createOffscreenBuffer( 20, 20 );
    }
    drawData command;
    command.arguments[0] = 0;
    command.arguments[1] = 0;
    command.arguments[2] = cursorBufferX;
    command.arguments[3] = cursorBufferY;
    command.arguments[4] = cursorBufferX;;
    command.arguments[5] = cursorBufferY;

    command.dataPointer = cursorBuffer;
    command.screen = &FRAME_BUFFER_SCREEN;

    copyBufferSection(command);
    //cout << "blah" << endl;
}