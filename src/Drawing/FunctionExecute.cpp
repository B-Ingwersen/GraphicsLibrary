#include "GraphicsLibrary/Drawing/FunctionExecute.h"
#include "GraphicsLibrary/Drawing/Functions.h"
#include "GraphicsLibrary/Drawing/Strings.h"
#include "GraphicsLibrary/RenderInfastructure/DrawingEvents.h"
#include "GraphicsLibrary/RenderInfastructure/WindowSections.h"

void DisplayWindow( drawData * window, screenData * screen ) {
	addEventExecute( drawWindow, 0, (*window).arguments, (*window).dataPointer, (*window).window, 1, screen);
}

void Background(int color, windowSection * window, screenData * screen) {
	int arguments[1] = {color};
	addEventExecute(drawBackground, 1, arguments, NULL, window, 1, screen);
}

void Rectangle(int x, int y, int width, int height, int color, windowSection * window, screenData * screen ) {
	int arguments[5] = {x, y, width, height, color};
	addEventExecute(drawRectangle, 2, arguments, NULL, window, 5, screen);
}

void Line(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen ) {
	int arguments[5] = {x1, y1, x2, y2, color};
	addEventExecute(drawLine, 3, arguments, NULL, window, 5, screen );
}

void Circle(int x, int y, int r, int color, windowSection * window, screenData * screen ) {
	int arguments[4] = {x, y, r, color};
	addEventExecute(&drawCircle, 4, arguments, NULL, window, 4, screen);
}

void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, windowSection * window, screenData * screen ) {
	int arguments[7] = {x1, y1, x2, y2, x3, y3, color};
	addEventExecute(drawTriangle, 5, arguments, NULL, window, 7, screen);
}

void Text(char * string, int x, int y, int width, int size, int color, int quality, windowSection * window, screenData * screen ) {
	void * dataPointer = string;
	int arguments[] = { x, y, width, size, color, 0 };
	/*if (quality >= 0 && quality <= 1) {
		addEventExecute(4 + quality, arguments, dataPointer, 5);
	}*/
	if (quality == 0) {
		addEventExecute(drawString, 6, arguments, dataPointer, window, 6, screen);
	}
	else if (quality == 1) {
		addEventExecute(betterQualString, 7, arguments, dataPointer, window, 6, screen);
	}
}

void CopyBuffer( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha ) {
	void * dataPointer = sourceBuffer;
	int arguments[] = { xSource, ySource, xTarget, yTarget, width, height };
	if ( alpha ) {
		addEventExecute( copyBufferSectionWithAlpha, 8, arguments, dataPointer, NULL, 6, targetBuffer);
	}
	else {
		addEventExecute( copyBufferSection, 8, arguments, dataPointer, NULL, 6, targetBuffer);
	}
}

void CopyBufferFit( screenData * sourceBuffer, screenData * targetBuffer, int xTarget, int yTarget, int width, int height, bool alpha ) {
	void * dataPointer = sourceBuffer;
	int arguments[] = { xTarget, yTarget, width, height };
	if ( alpha ) {
		addEventExecute( copyBufferScaledWithAlpha, 9, arguments, dataPointer, NULL, 4, targetBuffer);
	}
	else {
		addEventExecute( copyBufferScaled, 9, arguments, dataPointer, NULL, 4, targetBuffer);
	}
}

void GreyShade( int x, int y, int width, int height, int grayValue, int addValue, windowSection * window, screenData * screen ) {
	int arguments[6] = {x, y, width, height, grayValue, addValue };
	addEventExecute( drawGreyShade, 10, arguments, NULL, window, 6, screen);
}

void AlphaShade( int x, int y, int width, int height, int alpha, int color, windowSection * window, screenData * screen ) {
	int arguments[7] = {x, y, width, height, alpha, color };
	addEventExecute( drawAlphaShade, 10, arguments, NULL, window, 6, screen);
}

void Blur( int x, int y, int width, int height, int radius, int grayValue, int addValue, windowSection * window, screenData * screen ) {
	int arguments[7] = {x, y, width, height, radius, grayValue, addValue };
	addEventExecute( drawBlur, 10, arguments, NULL, window, 7, screen);
}

void VoidDraw() {
	if (currentEventQueue) {eventQueue2Index = 0;}
	else {eventQueue1Index = 0;}	
}
