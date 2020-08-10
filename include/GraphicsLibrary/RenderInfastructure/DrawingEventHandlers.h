
#ifndef _RENDER_INFASTRUCTURE___DRAWING_EVENT_HANDLERS_H_
#define _RENDER_INFASTRUCTURE___DRAWING_EVENT_HANDLERS_H_

#define HANDLER_DISABLED 0
#define GLOBAL_HANDLER 1
#define MOUSE_LOCATION_HANDLER 2

#define MOUSE_BUTTON 0
#define MOUSE_MOTION 1
#define RIGHT_BUTTON 2
#define MOUSE_DRAGGED 3
#define MOUSE_SCROLL 4
#define MOUSE_RELEASE 5

#define WINDOW_ENTER 6
#define WINDOW_EXIT 7

#define KEY_PRESS 8
#define LEAVE_MOUSE_FOCUS 9

#define RESIZE_FUNCTION 10
#define KEY_RELEASE 11

#include "GraphicsLibrary/Definitions.h"

void doNothingEventHandler();
void coverEventHandler(drawData * window);

void initializeEventHandlers();

void setHandlerFunction( int handlerID, void newHandler() );

void setHandlerState( int handlerID, int newHandlerState );

void disableHandler( int handlerID );

void callGlobalEventHandler( int handlerID );

void clearWindowEvents();

void callWindowEventHandler( drawData * handlingWindow, int handlerID );

void callEventHandler( int handlerID );

void addWindowEventHandler( drawData * handlingWindow, int handlerID, void handlerFunction( drawData * handlingWindow ) );

void deleteWindowEventHandler( drawData * handlingWindow, int handlerID );

#endif // _RENDER_INFASTRUCTURE___DRAWING_EVENT_HANDLERS_H_
















