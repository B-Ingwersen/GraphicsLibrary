
drawData * addBackground(int color, drawData * window) {
	int arguments[1] = { color };
	return createWindowObject( drawBackground, 1, arguments, NULL, 1, window );
}

drawData * addRectangle(int x, int y, int width, int height, int color, drawData * window) {
	int arguments[5] = {x, y, width, height, color};
	return createWindowObject( drawRectangle, 2, arguments, NULL, 5, window );
}

drawData * addLine(int x1, int y1, int x2, int y2, int color, drawData * window) {
	int arguments[5] = {x1, y1, x2, y2, color};
	return createWindowObject( drawLine, 3, arguments, NULL, 5, window );
}

drawData * addCircle(int x, int y, int r, int color, drawData * window) {
	int arguments[4] = { x, y, r, color };
	return createWindowObject( drawCircle, 4, arguments, NULL, 4, window );
}

drawData * addTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int color, drawData * window) {
	int arguments[7] = {x1, y1, x2, y2, x3, y3, color};
	return createWindowObject( drawTriangle, 5, arguments, NULL, 7, window );
}

drawData * addText(char * string, int x, int y, int width, int size, int color, int quality, drawData * window) {
	int arguments[6] = { x, y, width, size, color, 0 };
	if (quality == 1) {
		return createWindowObject( betterQualString, 6, arguments, string, 7, window );
	}
	else {
		return createWindowObject( drawString, 7, arguments, string, 7, window );
	}
}

drawData * addBlur( int x, int y, int width, int height, int radius, int grayValue, int addValue, drawData * window ) {
	int arguments[7] = {x, y, width, height, radius, grayValue, addValue };
	return createWindowObject( drawBlur, 8, arguments, NULL, 7, window );
}

drawData * addVertGradBack( int color1, int color2, drawData * window) {
	int arguments[2] = { color1, color2 };
	return createWindowObject( drawVertGradBack, 2, arguments, NULL, 2, window );
}

drawData * addHorizGradBack( int color1, int color2, drawData * window) {
	int arguments[2] = { color1, color2 };
	return createWindowObject( drawHorizGradBack, 2, arguments, NULL, 2, window );
}

drawData * addAlphaShade( int x, int y, int width, int height, int alpha, int color, drawData * window) {
	int arguments[7] = {x, y, width, height, alpha, color };
	return createWindowObject( drawAlphaShade, 1, arguments, NULL, 7, window);
}
