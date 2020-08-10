
#ifndef _DRAWING___FUNCTION_EXECUTE_H_
#define _DRAWING___FUNCTION_EXECUTE_H_

#include "GraphicsLibrary/Definitions.h"

void DisplayWindow( drawData * window, screenData * screen );

void Background(int color, windowSection * window, screenData * screen);

void Rectangle(int x, int y, int width, int height, int color, windowSection * window, screenData * screen );

void Line(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen );

void Circle(int x, int y, int r, int color, windowSection * window, screenData * screen );

void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, windowSection * window, screenData * screen );

void Text(char * string, int x, int y, int width, int size, int color, int quality, windowSection * window, screenData * screen );

void CopyBuffer( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha );

void CopyBufferFit( screenData * sourceBuffer, screenData * targetBuffer, int xTarget, int yTarget, int width, int height, bool alpha );

void GreyShade( int x, int y, int width, int height, int grayValue, int addValue, windowSection * window, screenData * screen );

void AlphaShade( int x, int y, int width, int height, int alpha, int color, windowSection * window, screenData * screen );

void Blur( int x, int y, int width, int height, int radius, int grayValue, int addValue, windowSection * window, screenData * screen );

void VoidDraw();

#endif // _DRAWING___FUNCTION_EXECUTE_H_