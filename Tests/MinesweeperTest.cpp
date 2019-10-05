#include <stdlib.h>

#include "../DrawingLibrary.h"
#include "../Extensions/Text.h"
#include "../Extensions/BasicGUIElements.h"
#include "../Extensions/WindowShadows.h"
#include "../Extensions/ThemeExp.h"

struct boardInfo {
	drawData * tile;
	int state;
	int value;
	char text[2];
};

boardInfo * board = NULL;
int boardWidth = 8;
int boardHeight = 8;
int numberOfMines = 10;
int padding = 2;
int tileTextSize;
int tileSize;
int tilesRevealed;
bool gameEnded;
drawData * gameParent;
drawData * bottomPanel;

bool gameNotYetStarted;

void gameScreen( drawData * callingWindow );
void selectionScreen( drawData * callingWindow );

#define MINE 9
#define UNCLICKED 0
#define REVEALED 1
#define FLAGGED 2

#define themeShadowDepth 10
#define themeShadowAlpha 90

drawData * backArrow( drawData * window, int x, int y, int size, void clickFunction( drawData * window ) ) {
	drawData * button = createButtonText2( window, x, y, size, size, noText, clickFunction, 1 );
	addTriangle( size / 4, size / 2, size * 3 / 4, size / 5, size * 3 / 4, size * 4 / 5, 0, button );

	return button;
}

drawData * exitButton( drawData * window, int x, int y, int size, void clickFunction( drawData * window ) ) {
	drawData * button = createButtonText2( window, x, y, size, size, noText, clickFunction, 1 );
	int textSize = size * 4 / 45;
	int width = getWindowWidth( button );
	addTriangle( width / 5, width / 5, width * 4 / 5, width * 4 / 5, width * 4 / 5 - textSize, width * 4 / 5, 0, button );
	addTriangle( width / 5, width / 5, width / 5 + textSize, width / 5, width * 4 / 5, width * 4 / 5, 0, button );
	addTriangle( width / 5, width * 4 / 5, width * 4 / 5, width / 5, width * 4 / 5 - textSize, width / 5, 0, button );
	addTriangle( width / 5, width * 4 / 5, width / 5 + textSize, width * 4 / 5, width * 4 / 5, width / 5, 0, button );
	return button;
}

void forTileAround( int tile, void tileFunction( int tile )) {
	int i = tile / boardHeight;		// x
	int j = tile % boardHeight;		// y

	if ( j > 0 ) { tileFunction( tile - 1 ); }
	if ( j < boardHeight - 1 ) { tileFunction( tile + 1 ); }
	if ( i > 0 ) { tileFunction( tile - boardHeight ); }
	if ( i < boardWidth - 1 ) { tileFunction( tile + boardHeight ); }
	if ( j > 0 && i > 0 ) {
		tileFunction( tile - boardHeight - 1);
	}
	if ( j > 0 && i < boardWidth - 1 ) {
		tileFunction( tile + boardHeight - 1 );
	}
	if ( j < boardHeight - 1 && i > 0 ) {
		tileFunction( tile - boardHeight + 1 );
	}
	if ( j < boardHeight - 1 && i < boardWidth - 1 ) {
		tileFunction( tile + boardHeight + 1 );
	}
}

void incrementTileValue( int tile ) {
	if ( board[tile].value != MINE ) {
		board[ tile ].value++;
	}
}

void generateBoard() {
	if ( board != NULL ) {
		delete board;
	}

	gameEnded = FALSE;
	tilesRevealed = 0;
	board = new boardInfo[ boardWidth * boardHeight ];

	if ( numberOfMines >= boardWidth * boardHeight ) {
		numberOfMines = boardWidth * boardHeight - 1;
	}

	int i;
	for ( i = 0; i < boardWidth * boardHeight; i++ ) {
		board[ i ].value = 0;
		board[ i ].state = UNCLICKED;
	}

	srand( time(NULL) );

	int minesAllocated = 0;
	while ( minesAllocated < numberOfMines ) {
		int mineIndex = rand() % ( boardWidth * boardHeight );
		if ( board[mineIndex].value != MINE ) {
			board[mineIndex].value = MINE;
			minesAllocated++;
			forTileAround( mineIndex, incrementTileValue );
		}
	}
}

// ############################################ TEXT #############################3
char welcomeMessage[] = "Welcome to Minesweeper on the New Graphics Library";
char enterMessage[] = "Enter";
char gamePlayMessage[] = "Minesweeper";
char gameOverMessage[] = "Game Over";
char winMessage[] = "You won!";
char restartMessage[] = "Play again";
char reselectMessage[] = "Select Game";
char customBoardMessage[] = "Custom board setup";
char exitMessage[] = "Are you sure that you want to leave?";

char defaultText1[] = "8x8"; char defaultText2[] = "10 mines";
char customText[] = "custom";

int titleTextSize = 4;
int menuTextSize = 3;
// ############################################ TEXT #############################3

void redrawAll() {
	displayWindow( parentWindow, onScreen );
}

int closeScreenStoreIndex;
void exitCloseScreen( drawData * callingWindow ) {
	gameParent -> arguments[0] = closeScreenStoreIndex;
	lastMouseWindow = gameParent;
	currentMouseWindow = gameParent;

	displayWindow( gameParent, onScreen );
}

char yesMessage[] = "   yes  ";
char noMessage[] =  "   no   ";

void resizeBlur( drawData * blur ) {

}

void closeScreen( drawData * callingWindow ) {
	closeScreenStoreIndex = gameParent -> arguments[0];

	drawData * coveringWindow = createBlurScreen( gameParent );

	drawData * text = drawCenteredText( coveringWindow, exitMessage, getWindowWidth( coveringWindow ) / 2, 100, 0, 4 );
	setResizeFunc( text, resizeCx );

	drawData * coveringWindow2 = createWindow( coveringWindow, 0, 0, getWindowWidth( coveringWindow ), getWindowHeight( coveringWindow ) );
	setResizeFunc( coveringWindow2, resizeHW );

	int textSize = 4;
	int width = textSize * 8 * 6 + 22;
	drawData * yesButton = createButtonFit( coveringWindow2, ( getWindowWidth( gameParent ) - width )/ 2 - 5, getWindowHeight( gameParent ) / 2, yesMessage, quitApplication, textSize );
	drawData * noButton = createButtonFit( coveringWindow2, ( getWindowWidth( gameParent ) + width )/ 2 + 5, getWindowHeight( gameParent ) / 2, noMessage, exitCloseScreen, textSize );
	setResizeFunc( yesButton, resizeCxy );
	setResizeFunc( noButton, resizeCxy );

	displayWindow( coveringWindow2, onScreen );
	displayWindow( coveringWindow, onScreen );
}

void drawMine( drawData * tile ) {
	removeButtonContents( tile );

	int x = tileSize / 2 - 1;
	int y = tileSize / 2 - 1;
	int r = (tileSize - 2 * padding) * 33 / 100;

	int t = (tileSize - 2 * padding) * 4 / 10;
	addTriangle( x - t, y, x, y - t, x + t, y, 0, tile );
	addTriangle( x - t, y, x, y + t, x + t, y, 0, tile );

	int l = (tileSize - 2 * padding) *  28 / 100;
	addRectangle( x - l, y - l, 2 * l, 2 * l, 0, tile );
}

void drawFlag( drawData * tile ) {
	removeButtonContents( tile );

	int x = tileSize / 2 - 1;
	int y = tileSize / 2 - 1;
	int d = ( tileSize - 2 * padding ) * 2 / 5;

	addRectangle( x + d / 2, y - d, tileTextSize, 2 * d, 0, tile );
	addTriangle( x + d / 2, y, x + d / 2, y - d, x - d / 2, y - d / 2, 0, tile );
}

void displayTile( int tileIndex ) {
	if ( gameNotYetStarted ) {
		gameNotYetStarted = false;
		//generateBoard();
	}

	drawData * tile = board[ tileIndex ].tile;
	int tileValue = board[ tileIndex ].value;

	disableButton( tile );
	board[ tileIndex ].state = REVEALED;

	if ( tileValue == MINE ) {
		drawMine( tile );
	}
	else if ( tileValue ) {
		removeButtonContents( tile );

		int x = tileSize / 2 - 1;
		int y = tileSize / 2 - 1;

		board[ tileIndex ].text[0] = (char) ( tileValue + 48 );
		board[ tileIndex ].text[1] = 0;
		drawCenteredText( tile, board[ tileIndex ].text, x, y, 0, tileTextSize );
	}
	displayWindow( tile, onScreen );

	tilesRevealed++;
}

drawData * addRestartButton( drawData * window ) {
	int textSize = 3;
	int textWidth = getTextLen( restartMessage ) * 6 * textSize;
	int x = getWindowWidth( window ) - textWidth / 2 - 20;
	return createButtonFit2( window, x, getWindowHeight( window ) - 37, restartMessage, gameScreen, textSize );
}

void blurBarResize1( drawData * blur ) {
	int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 48;
	int textHeightPixels = textSize * 13;

	blur -> arguments[0] = 0;
	blur -> arguments[1] = ( getWindowHeight( gameParent ) - textHeightPixels ) / 2;
	blur -> arguments[2] = ( getWindowWidth( gameParent ) );
	blur -> arguments[3] = textHeightPixels;
}
void blurBarResize2( drawData * blur ) {
	int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 54;
	int textHeightPixels = textSize * 13;

	blur -> arguments[0] = 0;
	blur -> arguments[1] = ( getWindowHeight( gameParent ) - textHeightPixels ) / 2;
	blur -> arguments[2] = ( getWindowWidth( gameParent ) );
	blur -> arguments[3] = textHeightPixels;
}
void blurBarResize11( drawData * blur ) {
	int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 48;
	int textHeightPixels = textSize * 13;

	blur -> arguments[1] = (getWindowWidth( gameParent ) - textSize * 8 * 6) / 2;
	blur -> arguments[2] = (getWindowHeight( gameParent ) - textSize * 9) / 2;
	blur -> arguments[3] = textSize * 8 * 6;
	blur -> arguments[4] = textSize * 9;
	reconfigureWindowSection( blur );
	drawData * text = (drawData *)(blur -> dataPointer) + 1;
	text -> arguments[3] = textSize;
}
void blurBarResize12( drawData * blur ) {
	int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 54;
	int textHeightPixels = textSize * 13;

	//deleteWindowContents( blur );
	//drawCenteredText( gameParent, gameOverMessage, getWindowWidth( gameParent ) / 2, getWindowHeight( gameParent ) / 2, 0xFF0000, textSize );
	blur -> arguments[1] = (getWindowWidth( gameParent ) - textSize * 9 * 6) / 2;
	blur -> arguments[2] = (getWindowHeight( gameParent ) - textSize * 9) / 2;
	blur -> arguments[3] = textSize * 9 * 6;
	blur -> arguments[4] = textSize * 9;
	reconfigureWindowSection( blur );
	drawData * text = (drawData *)(blur -> dataPointer) + 1;
	text -> arguments[3] = textSize;
}

void winGame() {
	if ( !gameEnded ) {
		int i;
		for ( i = 0; i < boardHeight * boardWidth; i++ ) {
			disableButton( board[i].tile );
		}

		int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 48;
		int textHeightPixels = textSize * 13;

		drawData * blur = addBlur( 0, ( getWindowHeight( gameParent ) - textHeightPixels ) / 2, ( getWindowWidth( gameParent ) ), textHeightPixels, 121, 1, 0x707070, gameParent );
		setResizeFunc( blur, blurBarResize1 );
		drawData * text = drawCenteredText( gameParent, winMessage, getWindowWidth( gameParent ) / 2, getWindowHeight( gameParent ) / 2, 0xFF00, textSize );
		setResizeFunc( text, blurBarResize11 );
		drawData * restartButton = addRestartButton( gameParent );
		setResizeFunc( getParentWindow(restartButton), resizeBR );

		displayWindow( gameParent, onScreen );
	}
}

void endGame() {
	int i;
	gameEnded = TRUE;
	for ( i = 0; i < boardHeight * boardWidth; i++ ) {
		displayTile( i );
	}

	int textSize = ( getWindowWidth( gameParent ) * 4 / 5) / 54;
	int textHeightPixels = textSize * 13;

	drawData * blur = addBlur( 0, ( getWindowHeight( gameParent ) - textHeightPixels ) / 2, ( getWindowWidth( gameParent ) ), textHeightPixels, 121, 1, 0x707070, gameParent );
	setResizeFunc( blur, blurBarResize2 );
	drawData * text = drawCenteredText( gameParent, gameOverMessage, getWindowWidth( gameParent ) / 2, getWindowHeight( gameParent ) / 2, 0xFF0000, textSize );
	setResizeFunc( text, blurBarResize12 );
	drawData * restartButton = addRestartButton( gameParent );
	setResizeFunc( getParentWindow(restartButton), resizeBR );
	

	displayWindow( gameParent, onScreen );
}

void expandTile( int tileIndex ) {
	drawData * tile = board[ tileIndex ].tile;
	int tileValue = board[ tileIndex ].value;

	if ( board[ tileIndex ].state != REVEALED ) {
		changeBackgroundColor( tile, 0xE0 );
		displayTile( tileIndex );

		if ( tileValue == 0 ) {
			forTileAround( tileIndex, expandTile );
		}
	}
}

void tileClick( drawData * tile ) {
	int tileIndex = tile -> arguments[15];
	int tileValue = board[ tileIndex ].value;

	if ( board[ tileIndex ].state == UNCLICKED ) {

		changeBackgroundColor( tile, 0xE0 );
		displayTile( tileIndex );

		if ( tileValue == MINE ) {
			endGame();
		}
		else if ( tileValue == 0 ) {
			forTileAround( tileIndex, expandTile );
		}
	}

	if ( tilesRevealed >= boardHeight * boardWidth - numberOfMines ) {
		winGame();
	}
}

void flagTile( drawData * tile ) {
	int tileIndex = tile -> arguments[15];

	if ( board[ tileIndex ].state == UNCLICKED ) {
		board[ tileIndex ].state = FLAGGED;
		drawFlag( tile );
	}
	else if ( board[ tileIndex ].state == FLAGGED ) {
		board[ tileIndex ].state = UNCLICKED;
		removeButtonContents( tile );
	}

	displayWindow( tile, onScreen );
}

void resizeTile( drawData * tile ) {
	removeButtonContents( tile );


	int tileIndex = tile -> arguments[15];
	int tileValue = board[ tileIndex ].value;
	if ( board[tileIndex].state == REVEALED ) {

		if ( tileValue == MINE ) {
			drawMine( tile );
		}
		else if ( tileValue ) {
			removeButtonContents( tile );

			int x = tileSize / 2 - 1;
			int y = tileSize / 2 - 1;

			board[ tileIndex ].text[0] = (char) ( tileValue + 48 );
			board[ tileIndex ].text[1] = 0;
			drawCenteredText( tile, board[ tileIndex ].text, x, y, 0, tileTextSize );
		}
	}
	else if ( board[ tileIndex ].state == FLAGGED ) {
		drawFlag( tile );
	}
}

void resizeBoard( drawData * gameWindow ) {
	tileSize = ( gameParent -> window -> Y2 - 150 ) / boardHeight;

	if ( tileSize * boardWidth >= gameParent -> window -> X2 ) {
		tileSize = gameParent -> window -> X2 / boardWidth;
	}

	int boardHeightPixels = tileSize * boardHeight;
	int boardWidthPixels = tileSize * boardWidth;

	tileTextSize = (tileSize - 2 * padding) * 4 / 45;	// times 0.8 for fit as well as / 9 for height in pixels
	gameWindow -> arguments[1] = (parentWindow -> window -> X2 - boardWidthPixels) / 2;
	gameWindow -> arguments[2] = 75;
	gameWindow -> arguments[3] = boardWidthPixels;
	gameWindow -> arguments[4] = boardHeightPixels;

	reconfigureWindowSection( gameWindow );

	int i,j;
	drawData * columns = (drawData *)(gameWindow -> dataPointer) + 1;
	for ( i = 0; i < boardWidth; i++ ) {
		columns -> arguments[1] = tileSize * i + padding;
		columns -> arguments[2] = 0;
		columns -> arguments[3] = tileSize - 2 * padding;
		columns -> arguments[4] = boardHeightPixels;

		reconfigureWindowSection( columns );
		drawData * tiles = (drawData *)(columns -> dataPointer) + 1;
		for ( j = 0; j < boardHeight; j++ ) {
			tiles -> arguments[1] = 0;
			tiles -> arguments[2] = tileSize * j + padding;
			tiles -> arguments[3] = tileSize - 2 * padding;
			tiles -> arguments[4] = tileSize - 2 * padding;

			reconfigureWindowSection( tiles );
			resizeTile(tiles);
			tiles++;
		}
		columns++;
	}
}


void gameScreen( drawData * callingWindow ) {
	gameNotYetStarted = true;
	deleteWindowContents( gameParent );
	addBackground( 0xFF, gameParent );

	drawData * message = drawCenteredText( gameParent, gamePlayMessage, (gameParent -> window -> X2) / 2, 37, 0, 4 );
	setResizeFunc( message, resizeCx );

	drawData * bkArrow = backArrow( gameParent, 20, 20, 80, selectionScreen );
	drawData * exButton = exitButton( gameParent, getWindowWidth( gameParent ) - 100, 20, 80, closeScreen );
	setResizeFunc( getParentWindow(exButton), resizeR );

	generateBoard();

	tileSize = ( gameParent -> window -> Y2 - 150 ) / boardHeight;

	if ( tileSize * boardWidth >= gameParent -> window -> X2 ) {
		tileSize = gameParent -> window -> X2 / boardWidth;
	}

	int boardHeightPixels = tileSize * boardHeight;
	int boardWidthPixels = tileSize * boardWidth;

	tileTextSize = (tileSize - 2 * padding) * 4 / 45;	// times 0.8 for fit as well as / 9 for height in pixels
	drawData * gameWindow = createWindow( gameParent, (parentWindow -> window -> X2 - boardWidthPixels) / 2, 75, boardWidthPixels, boardHeightPixels );
	bottomPanel = createWindow( gameParent, 0, parentWindow -> window -> Y2 - 75, parentWindow -> window -> X2, 75 );
	setResizeFunc( gameWindow, resizeBoard );

	int i,j;
	for ( i = 0; i < boardWidth; i++ ) {
		drawData * columnOfTiles = createWindow( gameWindow, tileSize * i + padding, 0, tileSize - 2 * padding, boardHeightPixels );
		for ( j = 0; j < boardHeight; j++ ) {
			drawData * tile = createButton( columnOfTiles, 0, tileSize * j + padding, tileSize - 2 * padding, tileSize - 2 * padding, noText, tileClick, 0 );
			setButtonColor( tile, 0xB0 );
			setButtonActiveColor( tile, 0xC0 );
			addWindowEventHandler( tile, RIGHT_BUTTON, flagTile );

			tile -> arguments[15] = i * boardHeight + j;
			board[ i * boardHeight + j].tile = tile;
		}
	}

	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	setHandlerState( RIGHT_BUTTON, MOUSE_LOCATION_HANDLER );

	displayWindow( parentWindow, onScreen );
}

char widthSelector[] = "Board width:";
char heightSelector[] = "Board height:";
char minesSelector[] = "Number of mines:";

drawData * minesCounter;

void widthCounterHandler( drawData * counter) {
	boardWidth = getCounterValue( counter );
	if ( numberOfMines >= boardWidth * boardHeight ) {
		numberOfMines = boardWidth * boardHeight - 1;
	}
	drawData * textArea = & ((drawData *) ( minesCounter -> dataPointer )) [ 3 ];
	textArea -> arguments[ 4 ] = boardWidth * boardHeight - 1;
	redrawCounter( minesCounter, 0, numberOfMines );
}
void heightCounterHandler( drawData * counter) {
	boardHeight = getCounterValue( counter );
	if ( numberOfMines >= boardWidth * boardHeight ) {
		numberOfMines = boardWidth * boardHeight - 1;
	}
	drawData * textArea = & ((drawData *) ( minesCounter -> dataPointer )) [ 3 ];
	textArea -> arguments[ 4 ] = boardWidth * boardHeight - 1;
	redrawCounter( minesCounter, 0, numberOfMines );
}
void minesCounterHandler( drawData * counter) {
	numberOfMines = getCounterValue( counter );
}

void customBoardSelection( drawData * callingWindow ) {
	deleteWindowContents( gameParent );
	addBackground( 0xFF, gameParent );

	drawData * message = drawCenteredText( gameParent, customBoardMessage, (gameParent -> window -> X2) / 2, 100, 0, 4 );
	setResizeFunc( message, resizeBCx );

	drawData * bkArrow = backArrow( gameParent, 20, 20, 80, selectionScreen );
	drawData * exButton = exitButton( gameParent, getWindowWidth( gameParent ) - 100, 20, 80, closeScreen );
	setResizeFunc( getParentWindow(exButton), resizeR );

	int textSize = 3;
	int width = 16 * textSize * 6 + 100;
	int height = textSize * 9 * 3 + 10;
	int x = ( gameParent -> window -> X2 - gameParent -> window -> X1 ) / 2;
	int y = ( gameParent -> window -> Y2 - gameParent -> window -> Y1 ) / 2;
	drawData * container = createWindow( gameParent, x - width / 2, y - height * 7 / 4 - textSize * 9 / 2, width, height * 7 / 2 );
	setResizeFunc( container, resizeCxy );

	addText( widthSelector, 0, height / 2 - textSize * 9 / 2, width, textSize, 0, 1, container );
	addText( heightSelector, 0, height * 7 / 4 - textSize * 9 / 2, width, textSize, 0, 1, container );
	addText( minesSelector, 0, height * 7 / 4 + height * 5 / 4 - textSize * 9 / 2, width, textSize, 0, 1, container );

	drawData * widthCounter = createCounter( container, width - 50, 0, 1, 40, 8, textSize, widthCounterHandler );
	drawData * heightCounter = createCounter( container, width - 50, height * 5 / 4, 1, 25, 8, textSize, heightCounterHandler );
	minesCounter = createCounter( container, width - 80, height * 5 / 2, 0, 1000, 10, textSize, minesCounterHandler );

	drawData * entButton = createButtonFit2( gameParent, ( gameParent -> window -> X2) / 2, ( gameParent -> window -> Y2) - 100, enterMessage, gameScreen, 4 );
	setResizeFunc( getParentWindow(entButton), resizeBCx );
	displayWindow( gameParent, onScreen );
}

void defaultGame( drawData * callingWindow ) {
	boardWidth = 8;
	boardHeight = 8;
	numberOfMines = 10;
	gameScreen( callingWindow );
}

void selectionScreen( drawData * callingWindow ) {
	deleteWindowContents( gameParent );
	addBackground( 0xFF, gameParent );

	drawData * message = drawCenteredText( gameParent, reselectMessage, (gameParent -> window -> X2) / 2, 100, 0, 4 );
	setResizeFunc( message, resizeCx );

	drawData * exButton = exitButton( gameParent, getWindowWidth( gameParent ) - 100, 20, 80, closeScreen );
	setResizeFunc( getParentWindow(exButton), resizeR );

	int padding = 20;
	int textSize = 4;

	int blockSize = 300;
	drawData * defaultButton = createButtonText2( gameParent, ( gameParent -> window -> X2 - padding ) / 2 + - blockSize,
		 ( gameParent -> window -> Y2 - blockSize - padding ) / 2, blockSize, blockSize, noText, defaultGame, textSize );
	drawCenteredText( defaultButton, defaultText1, blockSize / 2, blockSize / 2 - textSize * 9 * 3 / 4, 0, textSize );
	drawCenteredText( defaultButton, defaultText2, blockSize / 2, blockSize / 2 + textSize * 9 * 3 / 4, 0, textSize );
	setResizeFunc( getParentWindow(defaultButton), resizeCxy );

	drawData * customButton = createButtonText2( gameParent, ( gameParent -> window -> X2 + padding) / 2,
		 ( gameParent -> window -> Y2 - blockSize - padding ) / 2, blockSize, blockSize, noText, customBoardSelection, textSize );
	drawCenteredText( customButton, customText, blockSize / 2, blockSize / 2, 0, textSize );
	setResizeFunc( getParentWindow(customButton), resizeCxy );

	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	setHandlerState( RIGHT_BUTTON, MOUSE_LOCATION_HANDLER );
	setHandlerState( MOUSE_RELEASE, MOUSE_LOCATION_HANDLER );

	displayWindow( parentWindow, onScreen );
}

void welcomeScreen() {
	deleteWindowContents( parentWindow );
	drawData * parentBackground = addBackground( 0xFF, parentWindow );

	//createCounter( parentWindow, 500, 500, 4, 20, 8, 4 );

	drawData * entMessage = drawCenteredText( parentWindow, welcomeMessage, (parentWindow -> window -> X2) / 2, 100, 0, 4 );
	setResizeFunc( entMessage, resizeCx );
	drawData * enterButton = createButtonFit2( parentWindow, (parentWindow -> window -> X2) / 2, (parentWindow -> window -> Y2) - 100, enterMessage, selectionScreen, 4 );
	setResizeFunc( getParentWindow(enterButton), resizeBCx );

}

void createProgramStart(void * argument) {
	gameParent = parentWindow;
	welcomeScreen();
	displayWindow( parentWindow, onScreen );

	setHandlerState( MOUSE_BUTTON, MOUSE_LOCATION_HANDLER );
	setHandlerState( RIGHT_BUTTON, MOUSE_LOCATION_HANDLER );
	setHandlerState( MOUSE_RELEASE, MOUSE_LOCATION_HANDLER );
}

int main(int argc, char* args[])
{
	createDrawingEnvironment( createProgramStart, NULL );
	return 0;
};
