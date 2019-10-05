void copyDrawData( drawData * window1, drawData * window2 ) {
	window2 -> drawFunction = window1 -> drawFunction;
	window2 -> drawIdentifier = window1 -> drawIdentifier;

	int i;
	for ( i = 0; i < 16; i++ ) {
		window2 -> arguments[i] = window1 -> arguments[i];
	}

	window2 -> window = window1 -> window;
	window2 -> dataPointer = window1 -> dataPointer;
	window2 -> handlerFunctionsPointer = window1 -> handlerFunctionsPointer;
	window2 -> screen = window1 -> screen;
}

windowSection * createWindowSection(windowSection * parent, int x, int y, int width, int height) {
	windowSection * newWindow = new windowSection;

	newWindow -> x = parent -> x + x;
	newWindow -> y = parent -> y + y;

	(*newWindow).X1 = newWindow -> x;
	if ((*newWindow).X1 < (*parent).X1) {(*newWindow).X1 = (*parent).X1;}
	else if ((*newWindow).X1 > (*parent).X2) {(*newWindow).X1 = (*parent).X2 - 1;}

	(*newWindow).Y1 = newWindow -> y;
	if ((*newWindow).Y1 < (*parent).Y1) {(*newWindow).Y1 = (*parent).Y1;}
	else if ((*newWindow).Y1 > (*parent).Y2) {(*newWindow).Y1 = (*parent).Y2 - 1;}

	(*newWindow).X2 = newWindow -> x + width;
	if ((*newWindow).X2 < (*newWindow).X1) {(*newWindow).X2 = (*newWindow).X1;}
	else if ((*newWindow).X2 > (*parent).X2) {(*newWindow).X2 = (*parent).X2;}

	(*newWindow).Y2 = newWindow -> y + height;
	if ((*newWindow).Y2 < (*newWindow).Y1) {(*newWindow).Y2 = (*newWindow).Y1;}
	else if ((*newWindow).Y2 > (*parent).Y2) {(*newWindow).Y2 = (*parent).Y2;}

	return newWindow;
}

drawData * getParentWindow( drawData * window );
void reconfigureWindowSection( drawData * window ) {
	windowSection * newWindow = window -> window;
	int x = window -> arguments[1];
	int y = window -> arguments[2];
	int width = window -> arguments[3];
	int height = window -> arguments[4];

	drawData * parentWindow = getParentWindow( window );
	windowSection * parent = parentWindow -> window;

	newWindow -> x = parent -> x + x;
	newWindow -> y = parent -> y + y;

	(*newWindow).X1 = newWindow -> x;
	if ((*newWindow).X1 < (*parent).X1) {(*newWindow).X1 = (*parent).X1;}
	else if ((*newWindow).X1 > (*parent).X2) {(*newWindow).X1 = (*parent).X2 - 1;}

	(*newWindow).Y1 = newWindow -> y;
	if ((*newWindow).Y1 < (*parent).Y1) {(*newWindow).Y1 = (*parent).Y1;}
	else if ((*newWindow).Y1 > (*parent).Y2) {(*newWindow).Y1 = (*parent).Y2 - 1;}

	(*newWindow).X2 = newWindow -> x + width;
	if ((*newWindow).X2 < (*newWindow).X1) {(*newWindow).X2 = (*newWindow).X1;}
	else if ((*newWindow).X2 > (*parent).X2) {(*newWindow).X2 = (*parent).X2;}

	(*newWindow).Y2 = newWindow -> y + height;
	if ((*newWindow).Y2 < (*newWindow).Y1) {(*newWindow).Y2 = (*newWindow).Y1;}
	else if ((*newWindow).Y2 > (*parent).Y2) {(*newWindow).Y2 = (*parent).Y2;}
}

void drawWindow( drawData drawInformation ) {
	//cout << "draw" << endl;
	drawData * windowData = (drawData *) drawInformation.dataPointer;
	int maxIndex = drawInformation.arguments[0];

	int i;
	while ( maxIndex >= 127 ) {
		for ( i = 1; i < 127; i++ ) {
			windowData[i].screen = drawInformation.screen;
			if (windowData[i].drawFunction != NULL) {
				windowData[i].drawFunction( windowData[i] );
			}
		}
		windowData = (drawData*)(windowData[127].dataPointer);
		maxIndex -= 126;
	}
	for ( i = 1; i < maxIndex; i++ ) {
		windowData[i].screen = drawInformation.screen;
		if (windowData[i].drawFunction != NULL) {
			windowData[i].drawFunction( windowData[i] );
		}
	}
}

void callWindowEventHandler( drawData * handlingWindow, int handlerID );
void addWindowEventHandler( drawData * handlingWindow, int handlerID, void handlerFunction( drawData * handlingWindow ) );

void reconfigureWindowDimensions( drawData * window );
drawData * createWindow( drawData * parentWindow, int x, int y, int width, int height) {
	drawData * parentData = (drawData *) (* parentWindow).dataPointer;
	int index = (* parentWindow).arguments[0];
	//cout << index << "\t";
	while (index >= 127) {
		if (index == 127) {
			drawData * extendedData = new drawData[128];
			parentData[127].dataPointer = extendedData;
			extendedData[0].dataPointer = parentData;
			parentData = extendedData;
			index -= 126;
		}
		else {
			parentData = (drawData*)(parentData[127].dataPointer);
			index -= 126;
		}
	}

	drawData * returnPointer = &(parentData[index]);
	parentData[ index ].drawFunction = drawWindow;
	parentData[ index ].drawIdentifier = 0;		//identifier for window is 0
	parentData[ index ].arguments[0] = 1;
	parentData[ index ].arguments[1] = x;
	parentData[ index ].arguments[2] = y;
	parentData[ index ].arguments[3] = width;
	parentData[ index ].arguments[4] = height;
	parentData[ index ].handlerFunctionsPointer = NULL;					//No handler at first, must be added later.

	windowSection * windowArea = createWindowSection( (*parentWindow).window, x, y, width, height );
	parentData[ index ].window = windowArea;

	drawData * windowElements = new drawData[128];		// 128 max for now
	parentData[ index ].dataPointer = windowElements;
	windowElements[0].dataPointer = parentWindow; // first element gives pointer to parent

	addWindowEventHandler( returnPointer, 10, reconfigureWindowDimensions );

	(* parentWindow).arguments[0] += 1;
	return returnPointer;
}

drawData * getParentWindow( drawData * window ) {
	drawData * windowData = (drawData *) (* window).dataPointer;
	return (drawData * )(windowData[0].dataPointer);
}

drawData * createWindowObject( void (*drawEventFunction)(drawData self), int drawIdentifier, int * arguments, void * dataPointer, int numberOfArguments, drawData * parentWindow ) {

	drawData * parentData = (drawData *) (* parentWindow).dataPointer;
	int index = (* parentWindow).arguments[0];
	while (index >= 127) {
		if (index == 127) {
			drawData * extendedData = new drawData[128];
			parentData[127].dataPointer = extendedData;
			extendedData[0].dataPointer = parentData;
			parentData = extendedData;
			index -= 126;
		}
		else {
			parentData = (drawData*)(parentData[127].dataPointer);
			index -= 126;
		}
	}

	drawData * returnPointer = &(parentData[index]);
	parentData[ index ].drawFunction = drawEventFunction;
	parentData[ index ].drawIdentifier = drawIdentifier;	//identifier for window is 0
	parentData[ index ].window = (* parentWindow).window;
	parentData[ index ].dataPointer = dataPointer;
	parentData[ index ].handlerFunctionsPointer = NULL;

	int i;
	for ( i = 0; i < numberOfArguments; i++ ) {
		parentData[ index ].arguments[i] = arguments[i];
	}

	(* parentWindow).arguments[0] += 1;
	return returnPointer;
}

void clearWindowEvents();
void deleteWindowContents( drawData * window) {
	clearWindowEvents();

	drawData * windowData = (drawData *) (* window).dataPointer;
	int maxIndex = window -> arguments[0];

	int i;
	for ( i = 1; i < maxIndex; i++ ) {
		if ( windowData[i].drawIdentifier == 0 ) {
			deleteWindowContents( &windowData[i] );
		}
	}
	(* window).arguments[0] = 1;
}

void deleteWindow( drawData * window) {
	clearWindowEvents();

	drawData * windowData = (drawData *) (* window).dataPointer;
	int maxIndex = window -> arguments[0];

	int i;
	for ( i = 1; i < maxIndex; i++ ) {
		if ( windowData[i].drawIdentifier == 0 ) {
			deleteWindow( &windowData[i] );
		}
	}
	delete (drawData *)(window -> dataPointer);
	(* window).arguments[0] = 1;
}

// deletes all window data, ensuring no problems for extended pointers for sub windows
void deleteExtendedWindowData( drawData * window, int index ) {
	if (index < 127) { return; }
	drawData * windowData = (drawData *)(window -> dataPointer);
	windowData = (drawData*)(windowData[127].dataPointer);			// skip the first one
	index -= 126;
	while (index >= 127) {
		drawData * extendedData = (drawData*)(windowData[127].dataPointer);
		delete windowData;
		windowData = extendedData;
		index -= 126;
	}
	delete windowData;
}

bool checkCoordsInWindowSection( int x, int y, windowSection * window ) {
	if ( x < (*window).X1 ) { return false; }
	else if ( x >= (*window).X2 ) {return false; }
	else if ( y < (*window).Y1 ) {return false; }
	else if ( y >= (*window).Y2 ) {return false; }
	return true;
}

drawData * findWindowWithCoords( int x, int y, drawData * searchWindow ) {
	drawData * windowData = (drawData *) (* searchWindow).dataPointer;

	int index = searchWindow -> arguments[0];
	while (index >= 127) {
		windowData = (drawData*)(windowData[127].dataPointer);
		index -= 126;
	}
	index--;

	int i;
	for ( i = (* searchWindow).arguments[0] - 1; i > 0; i-- ) {
		if (index == 0) {
			windowData = (drawData*)(windowData[0].dataPointer);
			index = 126;
		}
		if ( windowData[index].drawIdentifier == 0 ) {
			if ( checkCoordsInWindowSection( x, y, windowData[index].window) ) {
				return findWindowWithCoords( x, y, &windowData[index] );
			}
		}
		index--;
	}
	return searchWindow;
}

drawData * findHandlerWindowWithCoords( int x, int y, drawData * searchWindow, int handlerID ) {

	drawData * window = findWindowWithCoords( x, y, searchWindow );

	while (window != parentWindow) {
		functionPointer * handlingFunctions =  (functionPointer *) ((*window).handlerFunctionsPointer);
		if (handlingFunctions[ handlerID ] != NULL ) {
			break;
		}
		window = getParentWindow( window );
	}

	return window;
}



















