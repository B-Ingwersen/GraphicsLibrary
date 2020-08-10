
#ifndef _EXTENSIONS___WINDOW_SHADOWS_H_
#define _EXTENSIONS___WINDOW_SHADOWS_H_

#include "GraphicsLibrary/Core.h"

void drawWindowShadowFunc( drawData drawInfo );

void drawInternalWindowShadow( drawData drawInfo );

void drawWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart, screenData * screen );

void drawWindowShadowNoRepeat( drawData * window, drawData * boundary, int d, int alphaStart, screenData * screen, int * lastDraw );

void addWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart );

void addWindowShadow2( drawData * window, drawData * boundary, int d, int alphaStart );

void addPartWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart, int top, int bottom, int right, int left );

void drawIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right, screenData * screen );

void DrawIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right, screenData * screen );

void addIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right );

#endif // _EXTENSIONS___WINDOW_SHADOWS_H_

