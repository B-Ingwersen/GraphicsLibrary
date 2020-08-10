
#ifndef _EXTENSIONS___BASIC_GUI_ELEMENTS_H_
#define _EXTENSIONS___BASIC_GUI_ELEMENTS_H_

#include "GraphicsLibrary/Core.h"

extern char noText[];
typedef void (*dataPointerFunction)(drawData *);
typedef void * extraFunc( drawData * window, char c );

void doNothingDrawData( drawData * window );

void changeBackgroundColor( drawData * window, int newBackground );

void removeButtonContents( drawData * button );

void setStateActive( drawData * window );

void setStateUnactive( drawData * window );

drawData * createButton( drawData * window, int x, int y, int width, int height, char * text, void clickFunction( drawData * window ), int textSize );

void setButtonColor( drawData * button, int color );

void setButtonActiveColor( drawData * button, int color );

void disableButton( drawData * button );

int getTextLen( char * text );

void recizeCxCenteredText( drawData * window );

drawData * drawCenteredText( drawData * window, char * text, int xCenter, int yCenter, int color, int textSize );

drawData * createButtonFit( drawData * window, int xCenter, int yCenter, char * text, void clickFunction( drawData * window ), int textSize );

int getWindowWidth( drawData * window );

int getWindowHeight( drawData * window );

void numberToText( int num, char * text, int digits );

void redrawCounter( drawData * counter, bool difference, int number );

int getCounterValue( drawData * counter );

void counterUp( drawData * button );

void counterDown( drawData * button );

drawData * createCounter( drawData * window, int x, int y, int min, int max, int defaultValue, int textSize, void testFunction( drawData * counter ) );

void resizeBlurScreenManip(drawData * window, drawData * parent);

void resizeBlurScreen( drawData * window );

drawData * createBlurScreen( drawData * window );

void textBoxHighlight(drawData * textBox);

void drawTextBoxCursor(drawData * textBox );

void hideTextBoxCursor(drawData * textBox );

void textBoxUnHighlight( drawData * textBox );

void focusTextBox( drawData * textBox );

void textBoxKeyHandler( drawData * textBox );

void textBoxLoseFocus( drawData * textBox );

drawData * createOneLineTextBox( drawData * window, int x, int y, int width, int height, int textSize, void extraFunction( drawData * textBox ) );

#endif // _EXTENSIONS___BASIC_GUI_ELEMENTS_H_

