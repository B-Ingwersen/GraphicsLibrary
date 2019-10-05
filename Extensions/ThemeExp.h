//g++ themeExp.cpp -lSDL2 -lpthread -o themeExp

void highlightButton2( drawData * window ) {
	drawData * container = getParentWindow( window );
	drawData * shadow = (drawData *)(window -> dataPointer) + 1;
	drawData * background = (drawData *)(window -> dataPointer) + 2;

	background -> arguments[0] = window -> colorInfo[1];
	shadow -> arguments[1] = 80;
	shadow -> arguments[2] = 15;
	shadow -> arguments[3] = 0;
	shadow -> arguments[4] = 8;
	shadow -> arguments[5] = 8;

	displayWindow( container, onScreen );
}

void unhighlightButton2( drawData * window ) {
	drawData * container = getParentWindow( window );
	drawData * shadow = (drawData *)(window -> dataPointer) + 1;
	drawData * background = (drawData *)(window -> dataPointer) + 2;
	background -> arguments[1] = 0;

	background -> arguments[0] = window -> colorInfo[0];
	shadow -> arguments[1] = 250;
	shadow -> arguments[2] = 17;
	shadow -> arguments[3] = 10;
	shadow -> arguments[4] = 15;
	shadow -> arguments[5] = 15;

	displayWindow( container, onScreen );
}

void mousePressButton2( drawData * window ) {
	drawData * container = getParentWindow( window );
	drawData * shadow = (drawData *)(window -> dataPointer) + 1;
	drawData * background = (drawData *)(window -> dataPointer) + 2;

	background -> arguments[0] = window -> colorInfo[2];
	background -> arguments[1] = 1;
	shadow -> arguments[1] = 0;
	shadow -> arguments[2] = 15;
	shadow -> arguments[3] = 0;
	shadow -> arguments[4] = 8;
	shadow -> arguments[5] = 8;

	displayWindow( container, onScreen );
	drawIntWindowShadow( window, 15, 60, 0, 8, 5, 5, onScreen );
}

void button2ClickFunction( drawData * window ) {
	drawData * background = (drawData *)(window -> dataPointer) + 2;
	if (background -> arguments[1]) {
		callWindowEventHandler( background, MOUSE_RELEASE );
	}
}

drawData * createButton2( drawData * window, int x, int y, int w, int h, void clickFunction( drawData * window ) ) {
	drawData * testWindowContainer = createWindow( window, x - 12, y - 5, w + 24, h + 25 );
	int i;
	int backgroundColor = 0;
	drawData * dataPointer = (drawData*)(window -> dataPointer);
	for ( i = 1; i < window -> arguments[0]; i++ ) {
		if ( (dataPointer + i) -> drawIdentifier == 1 ) {
			backgroundColor = (dataPointer + i) -> arguments[0];
			break;
		}
	}
	addBackground( backgroundColor, testWindowContainer );

	drawData * testWindow = createWindow( testWindowContainer, 12, 5, w, h );
	addPartWindowShadow( testWindow, testWindowContainer, 20, 250, 17, 10, 15, 15 );
	testWindow -> colorInfo[0] = 0xD4;
	testWindow -> colorInfo[1] = 0xC4;
	testWindow -> colorInfo[2] = 0xC0;

	drawData * background = addBackground( testWindow -> colorInfo[0], testWindow );
	background -> arguments[1] = 0;
	addWindowEventHandler( background, MOUSE_RELEASE, clickFunction );

	addWindowEventHandler( testWindow, WINDOW_ENTER, highlightButton2 );
	addWindowEventHandler( testWindow, WINDOW_EXIT, unhighlightButton2 );
	addWindowEventHandler( testWindow, MOUSE_BUTTON, mousePressButton2 );
	addWindowEventHandler( testWindow, MOUSE_RELEASE, button2ClickFunction );

	return testWindow;
}

void removeButtonContents2( drawData * button ) {
	button -> arguments[0] = 3;
}

void setButtonColor2( drawData * button, int color ) {
	drawData * background = (drawData*)(button->dataPointer) + 2;
	background -> arguments[0] = color;
	button -> colorInfo[0] = color;
}

void setButtonActiveColor2( drawData * button, int color ) {
	button -> colorInfo[1] = color;
}

void disableButton2( drawData * button ) {
	addWindowEventHandler( button, WINDOW_ENTER, NULL );
	addWindowEventHandler( button, WINDOW_EXIT, NULL );
	addWindowEventHandler( button, MOUSE_BUTTON, NULL );
	addWindowEventHandler( button, MOUSE_RELEASE, NULL );
}

drawData * createButtonFit2( drawData * window, int xCenter, int yCenter, char * text, void clickFunction( drawData * window ), int textSize ) {

	int textLen = getTextLen( text );
	int textWidth = textLen * 6 * textSize;
	int textHeight = 9 * textSize;
	int x = xCenter - ( textWidth + 20 ) / 2;
	int y = yCenter - ( textHeight + 20 ) / 2;
	int height = textHeight + 20;
	int width = textWidth + 20;

	drawData * myButton = createButton2( window, x, y, width, height, clickFunction );
	addText( text, 10, 10, width - 15, textSize, 0, 1, myButton );

	return myButton;
}

drawData * createButtonText2( drawData * window, int x, int y, int width, int height, char * text, void clickFunction( drawData * window ), int textSize ) {
	drawData * myButton = createButton2( window, x, y, width, height, clickFunction );

	addText( text, 10, 10, width - 20, textSize, 0, 1, myButton );

	return myButton;
}

//####################################################################################################################################################################


