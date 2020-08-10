#ifndef _DRAWING___OBJECTS_H_
#define _DRAWING___OBJECTS_H_

#include "GraphicsLibrary/Definitions.h"

drawData * addBackground(int color, drawData * window);

drawData * addRectangle(int x, int y, int width, int height, int color, drawData * window);

drawData * addLine(int x1, int y1, int x2, int y2, int color, drawData * window);

drawData * addCircle(int x, int y, int r, int color, drawData * window);

drawData * addTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, drawData * window);

drawData * addText(char * string, int x, int y, int width, int size, int color, int quality, drawData * window);

drawData * addBlur( int x, int y, int width, int height, int radius, int grayValue, int addValue, drawData * window );

drawData * addVertGradBack( int color1, int color2, drawData * window);

drawData * addHorizGradBack( int color1, int color2, drawData * window);

drawData * addAlphaShade( int x, int y, int width, int height, int alpha, int color, drawData * window);

#endif // _DRAWING___OBJECTS_H_