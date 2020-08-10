
#ifndef _EXTENSIONS___THEME_EXP_H_
#define _EXTENSIONS___THEME_EXP_H_

#include "GraphicsLibrary/Core.h"

void highlightButton2( drawData * window );

void unhighlightButton2( drawData * window );

void mousePressButton2( drawData * window );

void button2ClickFunction( drawData * window );

drawData * createButton2( drawData * window, int x, int y, int w, int h, void clickFunction( drawData * window ) );

void removeButtonContents2( drawData * button );

void setButtonColor2( drawData * button, int color );

void setButtonActiveColor2( drawData * button, int color );

void disableButton2( drawData * button );

drawData * createButtonFit2( drawData * window, int xCenter, int yCenter, char * text, void clickFunction( drawData * window ), int textSize );

drawData * createButtonText2( drawData * window, int x, int y, int width, int height, char * text, void clickFunction( drawData * window ), int textSize );

#endif // _EXTENSIONS___THEME_EXP_H_


