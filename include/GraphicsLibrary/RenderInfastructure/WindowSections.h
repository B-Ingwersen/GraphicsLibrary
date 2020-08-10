
#ifndef _RENDER_INFASTRUCTURE___WINDOW_SECTIONS_H_
#define _RENDER_INFASTRUCTURE___WINDOW_SECTIONS_H_

#include "GraphicsLibrary/Definitions.h"

void copyDrawData( drawData * window1, drawData * window2 );

windowSection * createWindowSection(windowSection * parent, int x, int y, int width, int height);

drawData * getParentWindow( drawData * window );

void reconfigureWindowSection( drawData * window );

void drawWindow( drawData drawInformation );

void callWindowEventHandler( drawData * handlingWindow, int handlerID );

void addWindowEventHandler( drawData * handlingWindow, int handlerID, void handlerFunction( drawData * handlingWindow ) );

void reconfigureWindowDimensions( drawData * window );

drawData * createWindow( drawData * parentWindow, int x, int y, int width, int height);

drawData * getParentWindow( drawData * window );

drawData * createWindowObject( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, int numberOfArguments, drawData * parentWindow );

void clearWindowEvents();

void deleteWindowContents( drawData * window);

void deleteWindow(drawData * window);

// deletes all window data, ensuring no problems for extended pointers for sub windows
void deleteExtendedWindowData( drawData * window, int index );

bool checkCoordsInWindowSection( int x, int y, windowSection * window );

drawData * findWindowWithCoords( int x, int y, drawData * searchWindow );

drawData * findHandlerWindowWithCoords( int x, int y, drawData * searchWindow, int handlerID );

#endif // _RENDER_INFASTRUCTURE___WINDOW_SECTIONS_H_



















