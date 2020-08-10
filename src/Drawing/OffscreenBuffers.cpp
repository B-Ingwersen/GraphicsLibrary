
#include "GraphicsLibrary/Drawing/OffscreenBuffers.h"
#include "GraphicsLibrary/Drawing/FunctionCalls.h"

screenData * createOffscreenBuffer( int windowWidth, int windowHeight ) {
	screenData * newScreen = new screenData;
	Uint32 * screenPixels = new Uint32[ windowWidth * windowHeight ];
	newScreen -> screen = screenPixels;
	newScreen -> windowWidth = windowWidth;
	newScreen -> windowHeight = windowHeight;
	return newScreen;
}

void deleteOffscreenBuffer(screenData * screen) {
	delete screen -> screen;
	delete screen;
}

screenData * duplicateBuffer( screenData * buffer ) {
	screenData * newScreen = new screenData;

	newScreen -> windowWidth = buffer -> windowWidth;
	newScreen -> windowHeight = buffer -> windowHeight;
	Uint32 * screenPixels = new Uint32[ (buffer -> windowWidth) * (buffer -> windowHeight) ];
	newScreen -> screen = screenPixels;

	copyBuffer( buffer, newScreen, 0, 0, 0, 0, buffer -> windowWidth, buffer -> windowHeight, false );

	return newScreen;
}

screenData * formatOffscreenBuffer( Uint32 * pixels, int width, int height ) {
	screenData * newScreen = new screenData;
	newScreen -> screen = pixels;
	newScreen -> windowWidth = width;
	newScreen -> windowHeight = height;
	return newScreen;
}

void resizeOffscreenBuffer( screenData * screen, int width, int height ) {
	delete screen -> screen;
	screen -> screen = new Uint32[ width * height ];
	screen -> windowWidth = width;
	screen -> windowHeight = height;
}
