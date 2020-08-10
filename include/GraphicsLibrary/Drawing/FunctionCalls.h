
#ifndef _DRAWING___FUNCTION_CALLS_H_
#define _DRAWING___FUNCTION_CALLS_H_

#include "GraphicsLibrary/Definitions.h"

void displayWindow( drawData * window, screenData * screen );

void displayWindowNoRepeat( drawData * window, screenData * screen );

void background(int color, windowSection * window, screenData * screen);

void rectangle(int x, int y, int width, int height, int color, windowSection * window, screenData * screen );

void line(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen );

void circle(int x, int y, int r, int color, windowSection * window, screenData * screen );

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, windowSection * window, screenData * screen );

void text(char * string, int x, int y, int width, int size, int color, int quality, windowSection * window, screenData * screen );

void copyBuffer( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha );

void copyBufferNoRepeat( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha, int * lastDraw);

void copyBufferFit( screenData * sourceBuffer, screenData * targetBuffer, int xTarget, int yTarget, int width, int height, bool alpha );

void greyShade( int x, int y, int width, int height, int grayValue, int addValue, windowSection * window, screenData * screen );

void alphaShade( int x, int y, int width, int height, int alpha, int color, windowSection * window, screenData * screen );

void blur( int x, int y, int width, int height, int radius, int grayValue, int addValue, windowSection * window, screenData * screen );

void voidDraw();

#endif // _DRAWING___FUNCTION_CALLS_H_