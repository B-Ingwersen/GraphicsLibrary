void displayWindow( drawData * window, screenData * screen ) {
	addEvent( drawWindow, 0, (*window).arguments, (*window).dataPointer, (*window).window, 1, screen);
}

void displayWindowNoRepeat( drawData * window, screenData * screen ) {
	addEventNoRepeat( drawWindow, 0, (*window).arguments, (*window).dataPointer, (*window).window, 1, screen, window -> colorInfo + 6);
}

void background(int color, windowSection * window, screenData * screen) {
	int arguments[1] = {color};
	addEvent(drawBackground, 1, arguments, NULL, window, 1, screen);
}

void rectangle(int x, int y, int width, int height, int color, windowSection * window, screenData * screen ) {
	int arguments[5] = {x, y, width, height, color};
	addEvent(drawRectangle, 2, arguments, NULL, window, 5, screen);
}

void line(int x1, int y1, int x2, int y2, int color, windowSection * window, screenData * screen ) {
	int arguments[5] = {x1, y1, x2, y2, color};
	addEvent(drawLine, 3, arguments, NULL, window, 5, screen );
}

void circle(int x, int y, int r, int color, windowSection * window, screenData * screen ) {
	int arguments[4] = {x, y, r, color};
	addEvent(&drawCircle, 4, arguments, NULL, window, 4, screen);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, windowSection * window, screenData * screen ) {
	int arguments[7] = {x1, y1, x2, y2, x3, y3, color};
	addEvent(drawTriangle, 5, arguments, NULL, window, 7, screen);
}

void text(char * string, int x, int y, int width, int size, int color, int quality, windowSection * window, screenData * screen ) {
	void * dataPointer = string;
	int arguments[] = { x, y, width, size, color, 0 };
	/*if (quality >= 0 && quality <= 1) {
		addEvent(4 + quality, arguments, dataPointer, 5);
	}*/
	if (quality == 0) {
		addEvent(drawString, 6, arguments, dataPointer, window, 6, screen);
	}
	else if (quality == 1) {
		addEvent(betterQualString, 7, arguments, dataPointer, window, 6, screen);
	}
}

void copyBuffer( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha ) {
	void * dataPointer = sourceBuffer;
	int arguments[] = { xSource, ySource, xTarget, yTarget, width, height };
	if ( alpha ) {
		addEvent( copyBufferSectionWithAlpha, 8, arguments, dataPointer, NULL, 6, targetBuffer);
	}
	else {
		addEvent( copyBufferSection, 8, arguments, dataPointer, NULL, 6, targetBuffer);
	}
}

void copyBufferNoRepeat( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height, bool alpha, int * lastDraw) {
	void * dataPointer = sourceBuffer;
	int arguments[] = { xSource, ySource, xTarget, yTarget, width, height };
	if ( alpha ) {
		addEventNoRepeat( copyBufferSectionWithAlpha, 8, arguments, dataPointer, NULL, 6, targetBuffer, lastDraw);
	}
	else {
		addEventNoRepeat( copyBufferSection, 8, arguments, dataPointer, NULL, 6, targetBuffer, lastDraw);
	}
}

void copyBufferFit( screenData * sourceBuffer, screenData * targetBuffer, int xTarget, int yTarget, int width, int height, bool alpha ) {
	void * dataPointer = sourceBuffer;
	int arguments[] = { xTarget, yTarget, width, height };
	if ( alpha ) {
		addEvent( copyBufferScaledWithAlpha, 9, arguments, dataPointer, NULL, 4, targetBuffer);
	}
	else {
		addEvent( copyBufferScaled, 9, arguments, dataPointer, NULL, 4, targetBuffer);
	}
}

void greyShade( int x, int y, int width, int height, int grayValue, int addValue, windowSection * window, screenData * screen ) {
	int arguments[6] = {x, y, width, height, grayValue, addValue };
	addEvent( drawGreyShade, 10, arguments, NULL, window, 6, screen);
}

void alphaShade( int x, int y, int width, int height, int alpha, int color, windowSection * window, screenData * screen ) {
	int arguments[7] = {x, y, width, height, alpha, color };
	addEvent( drawAlphaShade, 10, arguments, NULL, window, 6, screen);
}

void blur( int x, int y, int width, int height, int radius, int grayValue, int addValue, windowSection * window, screenData * screen ) {
	int arguments[7] = {x, y, width, height, radius, grayValue, addValue };
	addEvent( drawBlur, 10, arguments, NULL, window, 7, screen);
}

void voidDraw() {
	if (currentEventQueue) {eventQueue2Index = 0;}
	else {eventQueue1Index = 0;}	
}
