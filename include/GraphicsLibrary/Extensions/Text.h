
#ifndef _EXTENSIONS___TEXT_H_
#define _EXTENSIONS___TEXT_H_

#include "GraphicsLibrary/Core.h"

extern int * charDataList[];

void drawTextRing(drawData drawInformation);

drawData * addTextRing(int x, int y, int rOut, int rIn, int color, int mask, drawData * window);

void textCircle(drawData drawInformation);

void drawTextHShear( drawData drawInformation );

void drawTextVShear( drawData drawInformation );

drawData * addTextCircle(int x, int y, int rOut, int rIn, int color, int mask, drawData * window);

void drawChar( int x, int y, int color, int * charData, int Size, drawData stringData );

void drawString2( drawData drawInfo );

drawData * addText2(char * string, int x, int y, int width, int size, int color, int quality, drawData * window);

#endif // _EXTENSIONS___TEXT_H_
