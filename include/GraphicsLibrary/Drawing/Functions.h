
#ifndef _DRAWING___FUNCTIONS_H_
#define _DRAWING___FUNCTIONS_H_

#include "GraphicsLibrary/Definitions.h"

#define rmask 0xff0000
#define gmask 0xff00
#define bmask 0xff

int colorsToInt(int red, int green, int blue);

void drawBackground(drawData drawInformation);

void drawLine(drawData drawInformation);

void drawRectangle(drawData drawInformation);

void drawVertGradBack( drawData drawInformation );

void drawHorizGradBack( drawData drawInformation );

void drawGreyShade( drawData drawInformation );

void drawAlphaShade( drawData drawInformation );

void drawBlur( drawData drawInformation );

void drawCircle(drawData drawInformation);

void drawTriangle(drawData drawInformation);

void copyBufferSection(drawData drawInformation);

void copyBufferScaled(drawData drawInformation);

void copyBufferSectionWithAlpha(drawData drawInformation);

void copyBufferScaledWithAlpha(drawData drawInformation);

#endif // _DRAWING___FUNCTIONS_H_








