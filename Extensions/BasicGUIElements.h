
char noText[] = "";

void doNothingDrawData( drawData * window ) {}

void changeBackgroundColor( drawData * window, int newBackground ) {
	drawData * dataPointer = (drawData *) (* window).dataPointer;
	dataPointer[1].arguments[0] = newBackground;
}

void removeButtonContents( drawData * button ) {
	button -> arguments[0] = 2;
}

void setStateActive( drawData * window ) {
	changeBackgroundColor( window, window -> colorInfo[1] );
	displayWindow( window, onScreen );
}

void setStateUnactive( drawData * window ) {
	changeBackgroundColor( window, window -> colorInfo[0] );
	displayWindow( window, onScreen );
}

drawData * createButton( drawData * window, int x, int y, int width, int height, char * text, void clickFunction( drawData * window ), int textSize ) {
	drawData * myButton = createWindow( window, x, y, width, height );

	myButton -> colorInfo[0] = 0xD0;
	myButton -> colorInfo[1] = 0xC0;
	
	addBackground( myButton -> colorInfo[0], myButton );
	addText( text, 10, 10, width - 20, textSize, 0, 1, myButton );

	addWindowEventHandler( myButton, WINDOW_ENTER, setStateActive );
	addWindowEventHandler( myButton, WINDOW_EXIT, setStateUnactive );
	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	addWindowEventHandler( myButton, MOUSE_BUTTON, clickFunction );

	return myButton;
}

void setButtonColor( drawData * button, int color ) {
	changeBackgroundColor( button, color );
	button -> colorInfo[0] = color;
}

void setButtonActiveColor( drawData * button, int color ) {
	button -> colorInfo[1] = color;
}

void disableButton( drawData * button ) {
	addWindowEventHandler( button, WINDOW_ENTER, NULL );
	addWindowEventHandler( button, WINDOW_EXIT, NULL );
	addWindowEventHandler( button, MOUSE_BUTTON, NULL );
	addWindowEventHandler( button, RIGHT_BUTTON, NULL );
}

int getTextLen( char * text ) {
	int i = 0;
	for ( i = 0; text[i] != 0; i++ ) {}
	return i;
}

void recizeCxCenteredText( drawData * window ) {
	
}

drawData * drawCenteredText( drawData * window, char * text, int xCenter, int yCenter, int color, int textSize ) {
	int textLen = getTextLen( text );
	int textWidth = textLen * 6 * textSize;
	int textHeight = 9 * textSize;
	int x = xCenter - textWidth / 2;
	int y = yCenter - textHeight / 2;

	drawData * container = createWindow( window, x, y, textWidth, textHeight );
	addText( text, 0, 0, window -> window -> X2, textSize, color, 1, container );

	return container;
}

drawData * createButtonFit( drawData * window, int xCenter, int yCenter, char * text, void clickFunction( drawData * window ), int textSize ) {

	int textLen = getTextLen( text );
	int textWidth = textLen * 6 * textSize;
	int textHeight = 9 * textSize;
	int x = xCenter - ( textWidth + 20 ) / 2;
	int y = yCenter - ( textHeight + 20 ) / 2;
	int height = textHeight + 20;
	int width = textWidth + 20;

	drawData * myButton = createWindow( window, x, y, width, height );

	myButton -> colorInfo[0] = 0xD0;
	myButton -> colorInfo[1] = 0xC0;
	
	addBackground( myButton -> colorInfo[0], myButton );
	addText( text, 10, 10, width - 15, textSize, 0, 1, myButton );

	addWindowEventHandler( myButton, WINDOW_ENTER, setStateActive );
	addWindowEventHandler( myButton, WINDOW_EXIT, setStateUnactive );
	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	addWindowEventHandler( myButton, MOUSE_BUTTON, clickFunction );

	return myButton;
}

int getWindowWidth( drawData * window ) {
	return ( window -> window -> X2 - window -> window -> X1 );
}
int getWindowHeight( drawData * window ) {
	return ( window -> window -> Y2 - window -> window -> Y1 );
}

void numberToText( int num, char * text, int digits ) {
	text[ digits ] = 0;
	digits -= 1;
	int digit;
	while ( num > 0 && digits >= 0) {
		digit = num % 10;
		num = num / 10;
		text[ digits ] = (char) ( digit + 48 );
		digits -= 1;
	}
	while ( digits >= 0 ) {
		text[ digits ] = 32;
		digits -= 1;
	}
}

void redrawCounter( drawData * counter, bool difference, int number ) {
	drawData * textArea = & ((drawData *) ( counter -> dataPointer )) [ 3 ];

	int newValue;
	if ( difference ) {
		newValue = textArea -> arguments[5] + number;
	}
	else {
		newValue = number;
	}

	if ( newValue >= textArea -> arguments[7] && newValue <= textArea -> arguments[8] ) {
		textArea -> arguments[5] = newValue;
	}

	char * counterText = (char *) (((drawData *) ( textArea -> dataPointer )) [ 2 ].dataPointer );
	numberToText( textArea -> arguments[5], counterText, textArea -> arguments[6] );
	displayWindow( textArea, onScreen );
}

int getCounterValue( drawData * counter ) {
	drawData * textArea = & ((drawData *) ( counter -> dataPointer )) [ 3 ];
	return textArea -> arguments[5];
}

typedef void (* dataPointerFunction) ( drawData * window );

void counterUp( drawData * button ) {
	redrawCounter( getParentWindow( button ), TRUE, 1 );
	void * myFunction = ( (drawData *) (button -> dataPointer) )[ 3 ].dataPointer;
	if ( myFunction != NULL ) {
		( (dataPointerFunction) (myFunction) ) ( getParentWindow( button ) );
	}
}
void counterDown( drawData * button ) {
	redrawCounter( getParentWindow( button ), TRUE, -1 );
	void * myFunction = ( (drawData *) (button -> dataPointer) )[ 3 ].dataPointer;
	if ( myFunction != NULL ) {
		( (dataPointerFunction) (myFunction) ) ( getParentWindow( button ) );
	}
}


drawData * createCounter( drawData * window, int x, int y, int min, int max, int defaultValue, int textSize, void testFunction( drawData * counter ) ) {
	int minLength = (int) ( ceil( log10( abs( min ) + 1 ) ) );
	int maxLength = (int) ( ceil( log10( abs( max ) + 1 ) ) );
	int numLen;
	if ( minLength > maxLength ) {
		numLen = minLength;
	}
	else { numLen = maxLength; }

	int padding = 10;
	int width = textSize * numLen * 6 + padding;
	int height = textSize * 9 * 3 + padding;
	int subHeight = textSize * 9;
	drawData * counter = createWindow( window, x, y, width, height );

	drawData * upArrow = createButton( counter, 0, 0, width, subHeight, noText, counterUp, textSize );
	drawData * upTriangle = addTriangle( width / 2, subHeight / 5, width  / 5, subHeight * 4 / 5, width * 4 / 5, subHeight * 4 / 5, 0xA000, upArrow );
	upTriangle -> dataPointer = (void *) (testFunction);

	drawData * downArrow = createButton( counter, 0, subHeight * 2 + padding, width, subHeight, noText, counterDown, textSize );
	drawData * downTriangle = addTriangle( width / 2, subHeight * 4 / 5, width  / 5, subHeight / 5, width * 4 / 5, subHeight / 5, 0xA000, downArrow );
	downTriangle -> dataPointer = (void *) (testFunction);

	drawData * textArea = createWindow( counter, 0, textSize * 9, width, textSize * 9 + padding );
	addBackground( 0xE0, textArea );
	textArea -> arguments[5] = defaultValue;
	textArea -> arguments[6] = numLen;
	textArea -> arguments[7] = min;
	textArea -> arguments[8] = max;

	char * counterText = new char[12];
	numberToText( textArea -> arguments[5], counterText, numLen );
	drawData * text = addText( counterText, padding / 2, padding / 2, width - padding + 1, textSize, 0, 1, textArea );

	return counter;
}

void resizeBlurScreenManip(drawData * window, drawData * parent) {
	drawData * blur = (drawData*)(window -> dataPointer) + 1;

	window -> arguments[1] = parent -> arguments[1];
	window -> arguments[2] = parent -> arguments[2];
	window -> arguments[3] = parent -> arguments[3];
	window -> arguments[4] = parent -> arguments[4];

	blur -> arguments[0] = parent -> arguments[1];
	blur -> arguments[1] = parent -> arguments[2];
	blur -> arguments[2] = parent -> arguments[3];
	blur -> arguments[3] = parent -> arguments[4];
} void resizeBlurScreen( drawData * window ) {reconfigWindowDimsTemplate( window, resizeBlurScreenManip );}

drawData * createBlurScreen( drawData * window ) {
	drawData * coveringWindow = createWindow( window, 0, 0, getWindowWidth( window ), getWindowHeight( window ) );
	setResizeFunc( coveringWindow, resizeBlurScreen );

	lastMouseWindow = coveringWindow;
	currentMouseWindow = coveringWindow;

	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	addWindowEventHandler( coveringWindow, MOUSE_BUTTON, doNothingDrawData );
	addBlur( 0, 0, getWindowWidth( coveringWindow ), getWindowHeight( coveringWindow ), 121, 1, 0x404040, coveringWindow );

	return coveringWindow;
}

void textBoxHighlight(drawData * textBox) {
	if ( keyboardWindowFocus != textBox ) {
		setStateActive( textBox );
		char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );
		int index = textBox -> arguments[8];
	}
}

void drawTextBoxCursor(drawData * textBox ) {
	int index = textBox -> arguments[8];
	int textSize = ((drawData*)(textBox -> dataPointer))[2].arguments[15];
	((drawData*)(textBox -> dataPointer))[2].arguments[0] = 10 + 6 * textSize * index;
	((drawData*)(textBox -> dataPointer))[2].arguments[2] = 5 * textSize;
	((drawData*)(textBox -> dataPointer))[2].arguments[3] = 9 * textSize;
}

void hideTextBoxCursor(drawData * textBox ) {
	int index = textBox -> arguments[8];
	int textSize = ((drawData*)(textBox -> dataPointer))[2].arguments[15];
	((drawData*)(textBox -> dataPointer))[2].arguments[0] = 10 + 6 * textSize * index;
	((drawData*)(textBox -> dataPointer))[2].arguments[2] = 0;
	((drawData*)(textBox -> dataPointer))[2].arguments[3] = 0;
}

void textBoxUnHighlight( drawData * textBox ) {
	if ( keyboardWindowFocus != textBox ) {
		setStateUnactive( textBox );
		char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );
		int index = textBox -> arguments[8];

		hideTextBoxCursor( textBox);
	}
}

void focusTextBox( drawData * textBox ) {
	mouseWindowFocus = textBox;

	changeBackgroundColor( textBox, 0xFF );
	char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );

	drawTextBoxCursor( textBox );
	displayWindow( textBox, onScreen );
	keyboardWindowFocus = textBox;
}

typedef void * extraFunc( drawData * window, char c );

void textBoxKeyHandler( drawData * textBox ) {
	char c = handleKeyDown( lastKeyDown );
	int len = textBox -> arguments[7];
	int index = textBox -> arguments[8];
	if ( index < 126 && c >= 32 && c <= 126 ) {
		char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );

		memcpy( text + index + 1, text + index, len - index );
		text[ index ] = c;
		textBox -> arguments[7] = len + 1;
		textBox -> arguments[8] = index + 1;

	}
	else if ( c != 0 ) {
		if ( c == 8 && index > 0 ) {
			char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );

			memcpy( text + index - 1, text + index, len - index );
			textBox -> arguments[7] = len - 1;
			textBox -> arguments[8] = index - 1;
			text[ len - 1] = 0;
		}
		else if ( c == 127 && index < len ) {
			char * text = (char *) ( ( (drawData*)(textBox -> dataPointer))[3].dataPointer );
			memcpy( text + index, text + index + 1, len - index );
			textBox -> arguments[7] = len - 1;
			textBox -> arguments[8] = index;
			text[ len - 1] = 0;
		}
	}

	if ( c == -1 ) {
		if ( index < len ) {
			textBox -> arguments[8] = index + 1;
		}
		drawTextBoxCursor( textBox );
	}
	else if ( c == -2 ) {
		int index = textBox -> arguments[8];
		if ( index > 0 ) {
			textBox -> arguments[8] = index - 1;
		}
		drawTextBoxCursor( textBox );
	}
	else if ( c == -3 ) {
		textBox -> arguments[8] = 0;
		drawTextBoxCursor( textBox );
	}
	else if ( c == -4 ) {
		textBox -> arguments[8] = textBox -> arguments[7];
		drawTextBoxCursor( textBox );
	}

	changeBackgroundColor( textBox, 0xFF );
	drawTextBoxCursor( textBox );

	functionPointer extraFunction = ((functionPointer *) (((drawData*)(textBox -> dataPointer))[3].handlerFunctionsPointer))[KEY_PRESS];
	if ( extraFunction != 0 ) {
		extraFunction( textBox );
	}
	displayWindow( textBox, onScreen );
}

void textBoxLoseFocus( drawData * textBox ) {
	keyboardWindowFocus = NULL;
	textBoxUnHighlight( textBox );
}

drawData * createOneLineTextBox( drawData * window, int x, int y, int width, int height, int textSize, void extraFunction( drawData * textBox ) ) {
	drawData * textBox = createWindow( window, x, y, width, height );

	textBox -> colorInfo[0] = 0xD4;
	textBox -> colorInfo[1] = 0xC8;

	textBox -> arguments[7] = 0;
	textBox -> arguments[8] = 0;

	drawData * back = addBackground( textBox -> colorInfo[0], textBox );
	drawData * cursor = addRectangle( 10, (height - textSize * 9) / 2, 0, 0, 0xA0A0FF, textBox );
	cursor -> arguments[15] = textSize;

	addWindowEventHandler( textBox, WINDOW_ENTER, textBoxHighlight );
	addWindowEventHandler( textBox, WINDOW_EXIT, textBoxUnHighlight );

	char * text = new char[128];
	int i; for ( i = 0; i < 128; i++ ) { text[i] = 0; };

	//drawData * textObj = addText( text, 10, (height - textSize * 9) / 2 + textSize * 9, width - 10, 22, 0, 1, textBox );
	drawData * textObj = addText( text, 10, (height - textSize * 9) / 2, width - 10, textSize, 0, 1, textBox );
	textObj -> handlerFunctionsPointer = NULL;
	addWindowEventHandler( textObj, KEY_PRESS, extraFunction );

	addWindowEventHandler( textBox, MOUSE_BUTTON, focusTextBox );
	addWindowEventHandler( textBox, KEY_PRESS, textBoxKeyHandler );

	addWindowEventHandler( textBox, LEAVE_MOUSE_FOCUS, textBoxLoseFocus );

	return textBox;
}




