void (*sizeChangeFunction)() = NULL;

void setResizeFunc( drawData * window, void function(drawData * window) ) {
	addWindowEventHandler( window, 10, function );
}

pthread_spinlock_t sizeChangeSpinLock;
void reconfigureParentWindow(int width, int height) {
	pthread_spin_lock(&sizeChangeSpinLock);

	WINDOW_WIDTH_PARENT = width;
	WINDOW_HEIGHT_PARENT = height;

	int arg5 = parentWindow -> arguments[5];
	int arg6 = parentWindow -> arguments[6];
	int arg7 = parentWindow -> arguments[7];
	int arg8 = parentWindow -> arguments[8];
	parentWindow -> arguments[5] = parentWindow -> arguments[1];
	parentWindow -> arguments[6] = parentWindow -> arguments[2];
	parentWindow -> arguments[7] = parentWindow -> arguments[3];
	parentWindow -> arguments[8] = parentWindow -> arguments[4];

	onScreen -> windowWidth = width;
	onScreen -> windowHeight = height;

	if (GRAPHICS_MODE == GRAPHICS_MODE_FRAMEBUFFER) {
		onScreen -> screen = (Uint32 *)(FRAME_BUFFER_MASTER);
	}
	else if (GRAPHICS_MODE == GRAPHICS_MODE_SDL) {
		surface = SDL_GetWindowSurface( window );
		onScreen -> screen = (Uint32 *)(surface->pixels);
	}
	ALT_DOWN = false;
	CTRL_DOWN = false;
	leftButtonDown = false;
	caughtWindow = NULL;
	lastMouseWindow = NULL;
	currentMouseWindow = NULL;
	lastMouseWindowFocus = NULL;
	mouseWindowFocus = NULL;

	parentWindow -> arguments[3] = width;
	parentWindow -> arguments[4] = height;
	parentWindow -> window -> X2 = width - 1;
	parentWindow -> window -> Y2 = height - 1;

	drawData * windowData = (drawData*)(parentWindow -> dataPointer);
	int i;
	int maxIndex = parentWindow -> arguments[0];
	for ( i = 1; i < maxIndex; i++ ) {
		callWindowEventHandler( (drawData*)(parentWindow -> dataPointer) + i, 10 );
		if ( i == 126 ) { windowData = (drawData *)(windowData[127].dataPointer); i = 0; maxIndex -= 126; }
	}

	parentWindow -> arguments[5] = arg5;
	parentWindow -> arguments[6] = arg6;
	parentWindow -> arguments[7] = arg7;
	parentWindow -> arguments[8] = arg8;

	//reconfigureParentWindow( WINDOW_WIDTH_PARENT, WINDOW_HEIGHT_PARENT );
	if ( sizeChangeFunction != NULL ) { (*sizeChangeFunction)(); }

	pthread_spin_unlock(&sizeChangeSpinLock);
}

void reconfigureWindowDimensions( drawData * window ) {
	int arg5 = window -> arguments[5];
	int arg6 = window -> arguments[6];
	int arg7 = window -> arguments[7];
	int arg8 = window -> arguments[8];
	window -> arguments[5] = window -> arguments[1];
	window -> arguments[6] = window -> arguments[2];
	window -> arguments[7] = window -> arguments[3];
	window -> arguments[8] = window -> arguments[4];

	reconfigureWindowSection( window );

	int i;
	drawData * windowData = (drawData *) ( window -> dataPointer );
	int maxIndex = window -> arguments[0];
	for ( i = 1; i < maxIndex; i++ ) {
		if ( windowData[i].drawIdentifier == 0 ) {
			callWindowEventHandler( &windowData[i], 10 );
		}
		if ( i == 126 ) { windowData = (drawData *)(windowData[127].dataPointer); i = 0; maxIndex -= 126; }
	}

	window -> arguments[5] = arg5;
	window -> arguments[6] = arg6;
	window -> arguments[7] = arg7;
	window -> arguments[8] = arg8;
}

void reconfigWindowDimsTemplate( drawData * window, void manipFunc( drawData * window, drawData * parentWindow) ) {
	int arg5 = window -> arguments[5];
	int arg6 = window -> arguments[6];
	int arg7 = window -> arguments[7];
	int arg8 = window -> arguments[8];
	window -> arguments[5] = window -> arguments[1];
	window -> arguments[6] = window -> arguments[2];
	window -> arguments[7] = window -> arguments[3];
	window -> arguments[8] = window -> arguments[4];

	manipFunc( window, getParentWindow( window ) );

	reconfigureWindowSection( window );

	int i;
	drawData * windowData = (drawData *) ( window -> dataPointer );
	int maxIndex = window -> arguments[0];
	for ( i = 1; i < maxIndex; i++ ) {
		if ( windowData[i].drawIdentifier == 0 ) {
			callWindowEventHandler( &windowData[i], 10 );
		}
		if ( i == 126 ) { windowData = (drawData *)(windowData[127].dataPointer); i = 0; maxIndex -= 126; }
	}

	window -> arguments[5] = arg5;
	window -> arguments[6] = arg6;
	window -> arguments[7] = arg7;
	window -> arguments[8] = arg8;
}

void resizeHManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[4] += parentWindow -> arguments[4] - parentWindow -> arguments[8];
} void resizeH( drawData * window ) {reconfigWindowDimsTemplate( window, resizeHManip );}

void resizeWManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[3] += parentWindow -> arguments[3] - parentWindow -> arguments[7];
} void resizeW( drawData * window ) {reconfigWindowDimsTemplate( window, resizeWManip );}

void resizeHWManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[3] += parentWindow -> arguments[3] - parentWindow -> arguments[7];
	window -> arguments[4] += parentWindow -> arguments[4] - parentWindow -> arguments[8];
}void resizeHW( drawData * window ) { reconfigWindowDimsTemplate( window, resizeHWManip );}

void resizeBRManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[1] += parentWindow -> arguments[3] - parentWindow -> arguments[7];
	window -> arguments[2] += parentWindow -> arguments[4] - parentWindow -> arguments[8];
}void resizeBR( drawData * window ) { reconfigWindowDimsTemplate( window, resizeBRManip );}

void resizeBWManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[3] += parentWindow -> arguments[3] - parentWindow -> arguments[7];
	window -> arguments[2] += parentWindow -> arguments[4] - parentWindow -> arguments[8];
}void resizeBW( drawData * window ) { reconfigWindowDimsTemplate( window, resizeBWManip );}

void resizeCxManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[1] += (parentWindow -> arguments[3] - parentWindow -> arguments[7])/2;
}void resizeCx( drawData * window ) { reconfigWindowDimsTemplate( window, resizeCxManip );}

void resizeBCxManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[1] += (parentWindow -> arguments[3] - parentWindow -> arguments[7])/2;
	window -> arguments[2] += parentWindow -> arguments[4] - parentWindow -> arguments[8];
}void resizeBCx( drawData * window ) { reconfigWindowDimsTemplate( window, resizeBCxManip );}

void resizeCxyManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[1] += (parentWindow -> arguments[3] - parentWindow -> arguments[7])/2;
	window -> arguments[2] += (parentWindow -> arguments[4] - parentWindow -> arguments[8])/2;
}void resizeCxy( drawData * window ) { reconfigWindowDimsTemplate( window, resizeCxyManip );}

void resizeRManip( drawData * window, drawData * parentWindow ) {
	window -> arguments[1] += parentWindow -> arguments[3] - parentWindow -> arguments[7];
}void resizeR( drawData * window ) { reconfigWindowDimsTemplate( window, resizeRManip );}




