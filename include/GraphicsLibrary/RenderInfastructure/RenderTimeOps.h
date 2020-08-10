
#ifndef _RENDER_INFASTRUCTURE___RENDER_TIME_OPS_
#define _RENDER_INFASTRUCTURE___RENDER_TIME_OPS_

#include "GraphicsLibrary/Definitions.h"

void drawEventDeleteBuffer(drawData drawInformation);

void drawEventMoveWindow( drawData drawInformation );

void drawEventRepositionWindow( drawData drawInformation );

void drawWindowToBuffer( drawData drawInformation );

void displayWindowToBuffer( drawData * window );

void deleteBuffer( screenData * buffer );

void moveWindow( drawData * window, int deltaX, int deltaY );

void redimensionWindow( drawData * window, int x, int y, int width, int height );

#endif // _RENDER_INFASTRUCTURE___RENDER_TIME_OPS_