
#include "GraphicsLibrary/Extensions/WindowShadows.h"

void drawWindowShadowFunc( drawData drawInfo ) {
	//windowSection * window = drawInfo.window;
	//windowSection * boundary = ( windowSection * )( drawInfo.dataPointer );
	uint32_t * pixels = drawInfo.screen -> screen;
	int WINDOW_WIDTH = drawInfo .screen -> windowWidth;

	
	windowSection * window = ((drawData *)(drawInfo.dataPointer)) -> window;
	windowSection * boundary = drawInfo.window;
	if ( drawInfo.arguments[6] ) {
		boundary = ((drawData *)(drawInfo.dataPointer)) -> window;
		window = drawInfo.window;
	}

	int d = drawInfo.arguments[0];
	int alphaStart = drawInfo.arguments[1];
	int x1 = window -> X1;
	int y1 = window -> Y1;
	int x2 = window -> X2;
	int y2 = window -> Y2;

	int X1 = boundary -> X1 + drawInfo.arguments[4];
	int Y1 = boundary -> Y1 + drawInfo.arguments[2];
	int X2 = boundary -> X2 - drawInfo.arguments[5];
	int Y2 = boundary -> Y2 - drawInfo.arguments[3];

	int * alphaValues = new int[d];
	int * alphaValuesCorner = new int[ d * d ];
	//int * alphaValues2 = new int[d];

	int i;
	for ( i = 0; i < d; i++ ) {
		//alphaValues[i] = 256 - alphaStart * i / d;
		alphaValues[i] = 256 - alphaStart * ( d - i ) * ( d - i ) / ( d * d );
		int j;
		for ( j = 0; j < d; j++ ) {
			if ( i * i + j * j < d * d ) {
				float val = d - sqrt( i * i + j * j );
				alphaValuesCorner[ j * d + i ] = 256 - alphaStart * val * val / ( d * d );
			}
			else { alphaValuesCorner[ j * d + i ] = 256; }
		}
	}

	int x, y, xmax, ymax;

	xmax = X1;
	x = xmax - d + 1;
	y = Y1;
	ymax = Y2;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ xmax - i ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X2;
	xmax = x + d - 1;
	y = Y1;
	ymax = Y2;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ i - x ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X1;
	xmax = X2;
	ymax = Y1;
	y = ymax - d + 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ ymax - j ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X1;
	xmax = X2;
	y = Y2;
	ymax = y + d - 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ j - y ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	xmax = X1;
	ymax = Y1;
	x = xmax - d + 1;
	y = ymax - d + 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {
		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValuesCorner[ (ymax - j) * d + ( xmax - i ) ] ;
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}
	}
	x = X2;
	xmax = x + d - 1;
	ymax = Y1;
	y = ymax - d + 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {
		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValuesCorner[ (ymax - j) * d + ( i - x ) ] ;
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}
	}
	xmax = X1;
	y = Y2;
	ymax = y + d - 1;
	x = xmax - d + 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {
		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValuesCorner[ (j - y) * d + ( xmax - i ) ] ;
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}
	}
	x = X2;
	xmax = x + d - 1;
	y = Y2;
	ymax = y + d - 1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {
		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValuesCorner[ (j - y) * d + ( i - x ) ] ;
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}
	}

	delete alphaValues;
	delete alphaValuesCorner;
}

void drawInternalWindowShadow( drawData drawInfo ) {
	//windowSection * window = drawInfo.window;
	//windowSection * boundary = ( windowSection * )( drawInfo.dataPointer );
	Uint32 * pixels = drawInfo.screen -> screen;
	int WINDOW_WIDTH = drawInfo .screen -> windowWidth;

	//windowSection * boundary = drawInfo.window;

	int d = drawInfo.arguments[0];
	int alphaStart = drawInfo.arguments[1];
	int x1 = drawInfo.window -> X1;
	int y1 = drawInfo.window -> Y1;
	int x2 = drawInfo.window -> X2;
	int y2 = drawInfo.window -> Y2;

	int X1 = drawInfo.window -> X1;
	int Y1 = drawInfo.window -> Y1;
	int X2 = drawInfo.window -> X2;
	int Y2 = drawInfo.window -> Y2;

	int right = drawInfo.arguments[5];
	int top = drawInfo.arguments[2];
	int left =  drawInfo.arguments[4];
	int bot = drawInfo.arguments[3];

	int * alphaValues = new int[d];
	int * alphaValuesCorner = new int[ d * d ];
	//int * alphaValues2 = new int[d];

	int i;
	for ( i = 0; i < d; i++ ) {
		//alphaValues[i] = 256 - alphaStart * i / d;
		alphaValues[i] = 256 - alphaStart * ( d - i ) * ( d - i ) / ( d * d );
		int j;
		for ( j = 0; j < d; j++ ) {
			if ( i * i + j * j < d * d ) {
				float val = d - sqrt( i * i + j * j );
				alphaValuesCorner[ j * d + i ] = 256 - alphaStart * val * val / ( d * d );
			}
			else { alphaValuesCorner[ j * d + i ] = 256; }
		}
	}

	int x, y, xmax, ymax;

	xmax = X1 + d - 1 - left;
	x = X1;
	y = Y1;
	ymax = Y2;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ i - x + left ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X2 - d + 1 + right;
	xmax = X2;
	y = Y1;
	ymax = Y2;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ xmax - i + right ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X1;
	xmax = X2;
	ymax = Y1 + d - 1 - top;
	y = Y1;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ j - y + top ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	x = X1;
	xmax = X2;
	y = Y2 - d + 1 + bot;
	ymax = Y2;
	if ( x < x2 && y < y2 && xmax > x1 && ymax > y1 ) {

		if (x < x1) {x = x1;}
		if (y < y1) {y = y1;}
		if (xmax > x2) {xmax = x2;}
		if (ymax > y2) {ymax = y2;}
		int j = y;
		while (j < ymax) {
			int i = x;
			while (i < xmax) {
				int alpha = alphaValues[ ymax - j + bot ];
				int pixVal = pixels[WINDOW_WIDTH * j + i];
				int r = ( (pixVal & 0xFF0000) * alpha / 256 ) & 0xFF0000;
				int g = ( (pixVal & 0xFF00) * alpha / 256) & 0xFF00;
				int b = ( (pixVal & 0xFF) * alpha / 256) & 0xFF;
				pixels[WINDOW_WIDTH * j + i] = r + g + b;
				i++;
			}
			j++;
		}

	}

	delete alphaValues;
	delete alphaValuesCorner;
}


void drawWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart, screenData * screen ) {
	int arguments[7] = { d, alphaStart, 0,0,0,0,0 };//boundary -> window -> X1, boundary -> window -> Y1, boundary -> window -> X2, boundary -> window -> Y2, 0 };
	addEvent( drawWindowShadowFunc, 20, arguments, window, boundary -> window, 7, screen);
}
void drawWindowShadowNoRepeat( drawData * window, drawData * boundary, int d, int alphaStart, screenData * screen, int * lastDraw ) {
	int arguments[7] = { d, alphaStart, 0,0,0,0,0 };//boundary -> window -> X1, boundary -> window -> Y1, boundary -> window -> X2, boundary -> window -> Y2, 0 };
	addEventNoRepeat( drawWindowShadowFunc, 20, arguments, window, boundary -> window, 7, screen, lastDraw);
}

void addWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart ) {
	int arguments[7] = { d, alphaStart, 0, 0, 0, 0, 0 };
	createWindowObject( drawWindowShadowFunc, 20, arguments, boundary, 7, window );
}

void addWindowShadow2( drawData * window, drawData * boundary, int d, int alphaStart ) {
	int arguments[7] = { d, alphaStart, 0, 0, 0, 0, 1 };
	createWindowObject( drawWindowShadowFunc, 20, arguments, window, 7, boundary );
}

void addPartWindowShadow( drawData * window, drawData * boundary, int d, int alphaStart, int top, int bottom, int right, int left ) {
	int arguments[7] = { d, alphaStart, top, bottom, right, left, 0 };
	createWindowObject( drawWindowShadowFunc, 20, arguments, boundary, 7, window );
}

void drawIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right, screenData * screen ) {
	int arguments[6] = { d, alphaStart, top, bot, right, left };
	addEvent( drawInternalWindowShadow, 20, arguments, window, window -> window, 6, screen);
}
void DrawIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right, screenData * screen ) {
	int arguments[6] = { d, alphaStart, top, bot, right, left };
	addEventExecute( drawInternalWindowShadow, 20, arguments, window, window -> window, 6, screen);
}
void addIntWindowShadow( drawData * window, int d, int alphaStart, int top, int bot, int left, int right ) {
	int arguments[6] = { d, alphaStart, top, bot, right, left };
	createWindowObject( drawInternalWindowShadow, 20, arguments, window -> window, 6, window );
}

