
void drawEventDeleteBuffer(drawData drawInformation) {
	screenData * buffer = drawInformation.screen;
	delete buffer -> screen;
	delete buffer;
}

void drawEventMoveWindow( drawData drawInformation ) {
	int deltaX = drawInformation.arguments[0];
	int deltaY = drawInformation.arguments[1];
	drawData * window = (drawData *) (drawInformation.dataPointer);

	window -> arguments[1] += deltaX;
	window -> arguments[2] += deltaY;
	reconfigureWindowDimensions( window );
}

void drawEventRepositionWindow( drawData drawInformation ) {
	drawData * window = (drawData *) (drawInformation.dataPointer);

	window -> arguments[1] = drawInformation.arguments[0];
	window -> arguments[2] = drawInformation.arguments[1];
	window -> arguments[3] = drawInformation.arguments[2];
	window -> arguments[4] = drawInformation.arguments[3];

	//cout << "reconfig\t" << window -> arguments[3] << "\t" << window -> arguments[4] << endl;

	reconfigureWindowDimensions( window );
}

void drawWindowToBuffer( drawData drawInformation ) {
	drawData * window = (drawData *) (drawInformation.dataPointer);

	window -> arguments[1] -= window -> window -> x;
	window -> arguments[2] -= window -> window -> y;

	windowSection * storeWindow = window -> window;
	windowSection * newWindow = new windowSection;
	newWindow -> X1 = 0;
	newWindow -> Y1 = 0;
	newWindow -> X2 = window -> arguments[3];
	newWindow -> Y2 = window -> arguments[4];
	newWindow -> x = 0;
	newWindow -> y = 0;
	window -> window = newWindow;

	int i;
	drawData * windowData = (drawData *) ( window -> dataPointer );
	int maxIndex = window -> arguments[0];
	for ( i = 1; i < maxIndex; i++ ) {
		if ( windowData[i].drawIdentifier == 0 ) {
			reconfigureWindowDimensions( &windowData[i] );
		}
	}
	drawWindow( *window );

	window -> window = storeWindow;
	window -> arguments[1] += window -> window -> x;
	window -> arguments[2] += window -> window -> y;
	reconfigureWindowDimensions( window );

	delete newWindow;
}

void displayWindowToBuffer( drawData * window ) {
	void * dataPointer = window;
	addEvent( drawWindowToBuffer, 10, NULL, dataPointer, NULL, 0, NULL );
}

void deleteBuffer( screenData * buffer ) {
	addEvent( drawEventDeleteBuffer, 10, NULL, NULL, NULL, 0, buffer );
}

void moveWindow( drawData * window, int deltaX, int deltaY ) {
	int arguments[2] = { deltaX, deltaY };
	void * dataPointer = window;
	addEvent( drawEventMoveWindow, 11, arguments, dataPointer, NULL, 2, NULL);
}

void redimensionWindow( drawData * window, int x, int y, int width, int height ) {
	int arguments[4] = { x, y, width, height };
	void * dataPointer = window;
	addEvent( drawEventRepositionWindow, 11, arguments, dataPointer, NULL, 4, NULL);
}
