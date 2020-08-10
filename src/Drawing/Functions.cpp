
#include "GraphicsLibrary/Drawing/Functions.h"

int colorsToInt(int red, int green, int blue) {
	return 256 * 256 * 256 * red + 256 * 256 * green + 256 * blue;
}

void drawBackground(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;

	int color = drawInformation.arguments[0];

	int memStart = (* drawInformation.window).Y1 * WINDOW_WIDTH + (* drawInformation.window).X1;
	int memWidth = ( (* drawInformation.window).X2 - (* drawInformation.window).X1 ) * 4;

	int j;
	int jMax = (* drawInformation.window).Y2 - (* drawInformation.window).Y1;

	for ( j = 0; j < jMax; j++ ) {
		memset( pixels + memStart, color, memWidth );
		memStart += WINDOW_WIDTH;
	}
}

void drawLine(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x1 = drawInformation.arguments[0] + WINDOW.X1;
	int y1 = drawInformation.arguments[1] + WINDOW.Y1;
	int x2 = drawInformation.arguments[2] + WINDOW.X1;
	int y2 = drawInformation.arguments[3] + WINDOW.Y1;

	//cout << drawInformation.arguments[0] << "\t" << drawInformation.arguments[1] << "\t" << drawInformation.arguments[2] << "\t" << drawInformation.arguments[3] << endl;

	int color = drawInformation.arguments[4];

	int X1,Y1,X2,Y2;			// creates new coordinates that are sorted by y value
	X1 = x1; Y1 = y1;
	X2 = x2; Y2 = y2;
	if (y1 > y2) {				// Swaps the values if needed
		X1 = x2; Y1 = y2;
		X2 = x1; Y2 = y1;
	}

	int xdiff = X2 - X1;			// delta x and delta y values for drawing the line
	int ydiff = Y2 - Y1;
	int j = 0;				// the y value counter
	int yLimit = ydiff;			// the limit for the y value counter

	if (xdiff == 0 && ydiff == 0) {
		if (X1 < WINDOW.X2 && X1 >= WINDOW.X1 && Y1 < WINDOW.Y2 && Y1 >= WINDOW.Y1) {pixels[WINDOW_WIDTH * Y1 + X1] = color;}
		return;
	}
	if (yLimit + Y1 >= WINDOW.Y2) { yLimit = WINDOW.Y2 - Y1 - 1; } //Prevent screen overflow
	if (Y1 < WINDOW.Y1) { j = -Y1 + WINDOW.Y1; }
	if (abs(xdiff) < ydiff) {		// if more vertical, one pixel per row
		int i;
		while (j < yLimit) {
			i = j * xdiff / ydiff + X1;
			if (i < WINDOW.X2 && i >= WINDOW.X1) {pixels[WINDOW_WIDTH * (Y1 + j) + i] = color;}
			j++;
		}
	}
	else {
		int drawMemStart, drawMemEnd, i1, i2;
		if (ydiff == 0) {				//horizontal line
			if (Y1 >= WINDOW.Y1 && Y1 < WINDOW.Y2) {
				if (X1 >= WINDOW.X2 || X2 < WINDOW.X1) {return;}
				if (X1 < WINDOW.X1) {X1 = WINDOW.X1;}
				if (X2 >= WINDOW.X2) {X2 = WINDOW.X2- 1;}
				if (xdiff < 0) {
					drawMemStart = WINDOW_WIDTH * Y1 + X2;
					drawMemEnd = WINDOW_WIDTH * Y2 + X1;
				}
				else {
					drawMemStart = WINDOW_WIDTH * Y2 + X1;
					drawMemEnd = WINDOW_WIDTH * Y1 + X2;
				}
				while (drawMemStart < drawMemEnd) {
					pixels[drawMemStart] = color;
					drawMemStart++;
				}
			}
		}
		else {
			i2 = X1;
			while (j < yLimit) {
				i1 = i2;
				i2 = X1 + (j+1) * xdiff / ydiff;
				if (xdiff < 0) {
					if (i2 < WINDOW.X1) {i2 = WINDOW.X1;}
					if (i1 >= WINDOW.X2) {i1 = WINDOW.X2 - 1;}
					drawMemStart = WINDOW_WIDTH * (j + Y1) + i2;
					drawMemEnd = WINDOW_WIDTH * (j + Y1) + i1;
				}
				else {
					if (i1 < WINDOW.X1) {i1 = WINDOW.X1;}
					if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
					drawMemStart = WINDOW_WIDTH * (j + Y1) + i1;
					drawMemEnd = WINDOW_WIDTH * (j + Y1) + i2;
				}
				while (drawMemStart < drawMemEnd) {
					pixels[drawMemStart] = color;
					drawMemStart++;
				}
				j++;
			}
		}
	}
}

void drawRectangle(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.X1;
	int y = drawInformation.arguments[1] + WINDOW.Y1;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int color = drawInformation.arguments[4];

	if (x > WINDOW.X2 || y > WINDOW.Y2) {return;}
	int xmax = x + width;
	int ymax = y + height;
	if (xmax < WINDOW.X1 || ymax < WINDOW.Y1) {return;}
	if (xmax > WINDOW.X2) {xmax = WINDOW.X2;}
	if (ymax > WINDOW.Y2) {ymax = WINDOW.Y2;}
	if (x < WINDOW.X1) {x = WINDOW.X1;}
	if (y < WINDOW.Y1) {y = WINDOW.Y1;}
	int j = y;
	while (j < ymax) {
		int i = x;
		while (i < xmax) {
			pixels[WINDOW_WIDTH * j + i] = color;
			i++;
		}
		j++;
	}
}

void drawVertGradBack( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x = WINDOW.X1;
	int y = WINDOW.Y1;
	int width = WINDOW.X2 - WINDOW.X1;
	int height = WINDOW.Y2 - WINDOW.Y1;
	int color1 = drawInformation.arguments[0];
	int color2 = drawInformation.arguments[1];

	int red1 = ( color1 >> 16 ) & 0xFF;
	int green1 = ( color1 >> 8 ) & 0xFF;
	int blue1 = color1 & 0xFF;
	int red2 = ( color2 >> 16 ) & 0xFF;
	int green2 = ( color2 >> 8 ) & 0xFF;
	int blue2 = color2 & 0xFF;

	int xmax = x + width;
	int ymax = y + height;

	int j = y;
	while (j < ymax) {
		int i = x;
		int red = ( (j - y) * red2 + (height + y - j) * red1 ) / height & 0xFF;
		int green = ( (j - y) * green2 + (height + y - j) * green1 ) / height & 0xFF;
		int blue = ( (j - y) * blue2 + (height + y - j) * blue1 ) / height & 0xFF;
		int color = ( red << 16 ) + ( green << 8 ) + blue;

		while (i < xmax) {
			pixels[WINDOW_WIDTH * j + i] = color;
			i++;
		}
		j++;
	}
}

void drawHorizGradBack( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x = WINDOW.X1;
	int y = WINDOW.Y1;
	int width = WINDOW.X2 - WINDOW.X1;
	int height = WINDOW.Y2 - WINDOW.Y1;
	int color1 = drawInformation.arguments[0];
	int color2 = drawInformation.arguments[1];

	int red1 = ( color1 >> 16 ) & 0xFF;
	int green1 = ( color1 >> 8 ) & 0xFF;
	int blue1 = color1 & 0xFF;
	int red2 = ( color2 >> 16 ) & 0xFF;
	int green2 = ( color2 >> 8 ) & 0xFF;
	int blue2 = color2 & 0xFF;

	int xmax = x + width;
	int ymax = y + height;

	int * dataRow = new int[width];

	int i = x;
	while (i < xmax) {
		int red = ( (i - x) * red2 + (width + x - i) * red1 ) / width & 0xFF;
		int green = ( (i - x) * green2 + (width + x - i) * green1 ) / width & 0xFF;
		int blue = ( (i - x) * blue2 + (width + x - i) * blue1 ) / width & 0xFF;
		int color = ( red << 16 ) + ( green << 8 ) + blue;
		dataRow[ i - x ] = color;
		i++;
	}
	int j = y;
	while (j < ymax) {
		memcpy( pixels + WINDOW_WIDTH * j + x, dataRow, width * 4 );
		j++;
	}

	delete dataRow;
}

void drawGreyShade( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.X1;
	int y = drawInformation.arguments[1] + WINDOW.Y1;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int grayValue = drawInformation.arguments[4];
	int addValue = drawInformation.arguments[5];

	uint32_t andValue = 0xFFFFFF;
	if ( grayValue == 1 ) { andValue = 0xFEFEFE; }
	if ( grayValue == 2 ) { andValue = 0xFCFCFC; }
	if ( grayValue == 3 ) { andValue = 0xF8F8F8; }

	if (x > WINDOW.X2 || y > WINDOW.Y2) {return;}
	int xmax = x + width;
	int ymax = y + height;
	if (xmax < WINDOW.X1 || ymax < WINDOW.Y1) {return;}
	if (xmax > WINDOW.X2) {xmax = WINDOW.X2;}
	if (ymax > WINDOW.Y2) {ymax = WINDOW.Y2;}
	if (x < WINDOW.X1) {x = WINDOW.X1;}
	if (y < WINDOW.Y1) {y = WINDOW.Y1;}

	int j = y;
	while (j < ymax) {
		int i = x;
		while (i < xmax) {
			pixels[WINDOW_WIDTH * j + i] = ( ( pixels[WINDOW_WIDTH * j + i] & andValue ) >> grayValue ) + addValue;
			i++;
		}
		j++;
	}
}

void drawAlphaShade( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.X1;
	int y = drawInformation.arguments[1] + WINDOW.Y1;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int alpha = drawInformation.arguments[4];
	int color = drawInformation.arguments[5];

	int r = (256 - alpha) * ( color & 0xFF0000 );
	int g = (256 - alpha) * ( color & 0xFF00 );
	int b = (256 - alpha) * ( color & 0xFF );

	if (x > WINDOW.X2 || y > WINDOW.Y2) {return;}
	int xmax = x + width;
	int ymax = y + height;
	if (xmax < WINDOW.X1 || ymax < WINDOW.Y1) {return;}
	if (xmax > WINDOW.X2) {xmax = WINDOW.X2;}
	if (ymax > WINDOW.Y2) {ymax = WINDOW.Y2;}
	if (x < WINDOW.X1) {x = WINDOW.X1;}
	if (y < WINDOW.Y1) {y = WINDOW.Y1;}

	int j = y;
	while (j < ymax) {
		int i = x;
		while (i < xmax) {
			uint32_t sourcePix = pixels[ WINDOW_WIDTH * j + i ];

			int red = ( ( ( sourcePix & 0xFF0000 ) * alpha + r ) >> 8 ) & 0xFF0000;
			int green = ( ( ( sourcePix & 0xFF00 ) * alpha + g ) >> 8 ) & 0xFF00;
			int blue = ( ( ( sourcePix & 0xFF ) * alpha + b ) >> 8 ) & 0xFF;

			pixels[ WINDOW_WIDTH * j + i ] = red + green + blue;

			i++;
		}
		j++;
	}
}

void drawBlur( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.X1;
	int y = drawInformation.arguments[1] + WINDOW.Y1;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int radius = drawInformation.arguments[4];
	int grayValue = drawInformation.arguments[5];
	int addValue = drawInformation.arguments[6];

	uint32_t andValue = 0xFFFFFF;
	if ( grayValue == 1 ) { andValue = 0xFEFEFE; }
	if ( grayValue == 2 ) { andValue = 0xFCFCFC; }
	if ( grayValue == 3 ) { andValue = 0xF8F8F8; }

	uint32_t * buffer = new uint32_t[ WINDOW_WIDTH * WINDOW_HEIGHT ];

	if (x > WINDOW.X2 || y > WINDOW.Y2) {return;}
	int xmax = x + width;
	int ymax = y + height;
	if (xmax < WINDOW.X1 || ymax < WINDOW.Y1) {return;}
	if (xmax > WINDOW.X2) {xmax = WINDOW.X2;}
	if (ymax > WINDOW.Y2) {ymax = WINDOW.Y2;}
	if (x < WINDOW.X1) {x = WINDOW.X1;}
	if (y < WINDOW.Y1) {y = WINDOW.Y1;}


	int j = y; int i;
	while (j < ymax) {
		//i = x + radius / 2;
		i = x;
		int I1 = x + radius / 2;
		int I2 = xmax - radius / 2 - 1;

		int r;
		uint32_t rvalue = 0;
		uint32_t gvalue = 0;
		uint32_t bvalue = 0;
		uint32_t radd = 0; uint32_t rsub = 0;
		uint32_t gadd = 0; uint32_t gsub = 0;
		uint32_t badd = 0; uint32_t bsub = 0;

		int rback = radius / 2;
		int rfront = rback + 1;
		int div = ( rfront + 1 ) * rfront / 2;;

		int index0 = WINDOW_WIDTH * j + x;
		int value0;

		for ( r = 0; r < rfront; r++ ) {
			value0 = pixels[ index0 ];

			radd += (rmask & value0) >> 8;
			gadd += gmask & value0;
			badd += bmask & value0;

			rvalue += (( rmask & value0 ) * ( rfront - r )) >> 8;
			gvalue += ( gmask & value0 ) * ( rfront - r );
			bvalue += ( bmask & value0 ) * ( rfront - r );

			index0++;
		}

		rsub = (rmask & pixels[WINDOW_WIDTH * j + x ]) >> 8;
		gsub = gmask & pixels[WINDOW_WIDTH * j + x ];
		bsub = bmask & pixels[WINDOW_WIDTH * j + x ];

		int index1 = WINDOW_WIDTH * j + i + rfront;
		int index2 = WINDOW_WIDTH * j + i;
		int index3 = WINDOW_WIDTH * j + i + 1;
		int index4 = WINDOW_WIDTH * j + i - rback;

		int value1;
		int value2;
		int value3;
		int value4;

		while ( i < xmax ) {
			buffer[ index2 ] = (( (rvalue / div) << 8) & rmask) + (( gvalue / div ) & gmask) + (( bvalue / div ) & bmask);

			if ( i <= I2 ) {
				value1 = pixels[ index1 ];
				radd += (rmask & value1) >> 8;
				gadd += (gmask & value1);
				badd += (bmask & value1);
			}
			else { div += I2 - i; }

			value2 = pixels[ index2 ];
			radd -= (rmask & value2) >> 8;
			gadd -= (gmask & value2);
			badd -= (bmask & value2);

			value3 = pixels[ index3 ];
			rsub += (rmask & value3) >> 8;
			gsub += (gmask & value3);
			bsub += (bmask & value3);

			if ( i >= I1 ) {
				value4 = pixels[ index4 ];
				rsub -= (rmask & value4) >> 8;
				gsub -= (gmask & value4);
				bsub -= (bmask & value4);
			}
			else { div += I1 - i; }

			rvalue += ( radd - rsub );
			gvalue += ( gadd - gsub );
			bvalue += ( badd - bsub );

			index1++;
			index2++;
			index3++;
			index4++;

			i++;
		}
		j++;
	}
	i = x;
	while (i < xmax) {
		//j = y + radius / 2;
		j = y;
		int J1 = y + radius / 2;
		int J2 = ymax - radius / 2 - 1;

		int r;
		uint32_t rvalue = 0;
		uint32_t gvalue = 0;
		uint32_t bvalue = 0;
		uint32_t radd = 0; uint32_t rsub = 0;
		uint32_t gadd = 0; uint32_t gsub = 0;
		uint32_t badd = 0; uint32_t bsub = 0;

		int rback = radius / 2;
		int rfront = rback + 1;
		int div = ( rfront + 1 ) * rfront / 2; //( radius / 2 + 1 ) * ( radius / 2 + 1 );

		int index0 = WINDOW_WIDTH * y + i;
		int value0;

		for ( r = 0; r < rfront; r++ ) {
			value0 = buffer[ index0 ];

			radd += (rmask & value0) >> 8;
			gadd += gmask & value0;
			badd += bmask & value0;

			rvalue += (( rmask & value0 ) * ( rfront - r )) >> 8;
			gvalue += ( gmask & value0 ) * ( rfront - r );
			bvalue += ( bmask & value0 ) * ( rfront - r );

			index0 += WINDOW_WIDTH;
		}

		rsub = (rmask & buffer[WINDOW_WIDTH * y + i ]) >> 8;
		gsub = gmask & buffer[WINDOW_WIDTH * y + i ];
		bsub = bmask & buffer[WINDOW_WIDTH * y + i ];

		int index1 = WINDOW_WIDTH * ( j + rfront ) + i;
		int index2 = WINDOW_WIDTH * j + i;
		int index3 = WINDOW_WIDTH * ( j + 1 ) + i;
		int index4 = WINDOW_WIDTH * ( j - rback ) + i;

		int value1;
		int value2;
		int value3;
		int value4;

		while (j < ymax ) {
			pixels[ index2 ] = (( (((rvalue / div) << 8) & rmask) + ((gvalue / div) & gmask) + ((bvalue / div) & bmask) & andValue ) >> grayValue ) + addValue ;
			//pixels[ index2 ] = (( ((rvalue / div) & rmask) + ((gvalue / div) & gmask) + ((bvalue / div) & bmask) & 0xFCFCFC ) >> 2 ) + 0x606060 ;

			if ( j <= J2 ) {
				value1 = buffer[ index1 ];
				radd += (rmask & value1) >> 8;
				gadd += (gmask & value1);
				badd += (bmask & value1);
			}
			else { div -= j - J2; }

			value2 = buffer[ index2 ];
			radd -= (rmask & value2) >> 8;
			gadd -= (gmask & value2);
			badd -= (bmask & value2);

			value3 = buffer[ index3 ];
			rsub += (rmask & value3) >> 8;
			gsub += (gmask & value3);
			bsub += (bmask & value3);

			if ( j >= J1 ) {
				value4 = buffer[ index4 ];
				rsub -= (rmask & value4) >> 8;
				gsub -= (gmask & value4);
				bsub -= (bmask & value4);
			}
			else { div += J1 - j; }

			rvalue += ( radd - rsub );
			gvalue += ( gadd - gsub );
			bvalue += ( badd - bsub );

			index1 += WINDOW_WIDTH;
			index2 += WINDOW_WIDTH;
			index3 += WINDOW_WIDTH;
			index4 += WINDOW_WIDTH;

			j++;
		}

		i++;
	}

	delete buffer;
}

void drawCircle(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.x;
	int y = drawInformation.arguments[1] + WINDOW.y;
	int r = drawInformation.arguments[2];
	int color = drawInformation.arguments[3];

	int i1, i2, drawMemStart, drawMemEnd, yMax;
	float rSquared = (float)(r * r);
	int j = -r;
	yMax = r;
	if (y - r < WINDOW.Y1) {j = -y + WINDOW.Y1;}
	if (y + yMax >= WINDOW.Y2) {yMax = WINDOW.Y2 - y - 1;}
	while (j < yMax) {
		i1 = (int)(sqrt(rSquared - (float)(j * j)));
		i2 = i1 + x;
		i1 = x - i1;
		if (i1 < WINDOW.X1) {i1 = WINDOW.X1;}
		if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * (j + y) + i1;
		drawMemEnd = WINDOW_WIDTH * (j + y) + i2;
		while (drawMemStart < drawMemEnd) {
			pixels[drawMemStart] = color;
			drawMemStart++;
		}
		j++;

	}
}

/*void drawTriangle(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x1 = drawInformation.arguments[0] + WINDOW.X1;
	int y1 = drawInformation.arguments[1] + WINDOW.Y1;
	int x2 = drawInformation.arguments[2] + WINDOW.X1;
	int y2 = drawInformation.arguments[3] + WINDOW.Y1;
	int x3 = drawInformation.arguments[4] + WINDOW.X1;
	int y3 = drawInformation.arguments[5] + WINDOW.Y1;
	int color = drawInformation.arguments[6];

	int X1, Y1, X2, Y2, X3, Y3;
	//X1 = 0;
	//Assign the uppercase variables according to the y-values: Y1 < Y2 < Y3
	if (y2 > y1) {
		if (y2 > y3) {
			X2 = x2;
			Y2 = y2;
			if (y3 > y1) {X3 = x3; Y3 = y3; X1 = x1; Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x3;Y1 = y3;}
		}
		else {
			X2 = x3;
			Y2 = y3;
			if (y2 > y1) {X3 = x2;Y3 = y2;	X1 = x1;Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x2;Y1 = y2;}
		}
	}
	else {
		if (y1 > y3) {
			X2 = x1;
			Y2 = y1;
			if (y3 > y2) {X3 = x3;Y3 = y3;X1 = x2;Y1 = y2;}
			else {X3 = x2;Y3 = y2;X1 = x3;Y1 = y3;}
		}
		else {
			X2 = x3;
			Y2 = y3;
			if (y2 > y1) {X3 = x2;	Y3 = y2;X1 = x1;Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x2;Y1 = y2;}
		}
	}


	int xdiff1 = X2 - X1; int ydiff1 = Y2 - Y1; //Line 1: (X1,Y1) -> (X2,Y2)
	int xdiff2 = X3 - X1; int ydiff2 = Y3 - Y1; //Line 2: (X1,Y1) -> (X3,Y3)
	int xdiff3 = X2 - X3; int ydiff3 = Y2 - Y3; //Line 3: (X2,Y2) -> (X3,Y3)
	int xDiffStore = xdiff1; // incase values are changed, store them before executing
	int yDiffStore = ydiff1;

	int yLimit = ydiff2;
	int j = 0;
	int i1, i2, drawMemStart, drawMemEnd; //drawing pointers for drawing horizontal segments

	if (ydiff2 != 0) {
		if ((float)(xdiff1) / (float)(ydiff1) > (float)(xdiff2) / (float)(ydiff2)) {  // swap x and y if one slope is greater than the other
			xdiff1 = xdiff2; xdiff2 = xDiffStore;
			ydiff1 = ydiff2; ydiff2 = yDiffStore;
		}

		if (yLimit + Y1 >= WINDOW_HEIGHT) { yLimit = WINDOW_HEIGHT - Y1 - 1; } //Prevent screen overflow
		if (Y1 < WINDOW.Y1) { j = -Y1 + WINDOW.Y1; }

		while (j <= yLimit) {
			i1 = j * xdiff1 / ydiff1 + X1;	// calculate start and endpoints of line segment
			i2 = j * xdiff2 / ydiff2 + X1;
			if (i1 < WINDOW.X1) {i1 = WINDOW.Y1;}
			if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
			drawMemStart = WINDOW_WIDTH * (j + Y1) + i1;
			drawMemEnd = WINDOW_WIDTH * (j + Y1) + i2;
			while (drawMemStart < drawMemEnd) {
				pixels[drawMemStart] = color;
				drawMemStart++;
			}
			j++;
		}
	}
	if (ydiff3 != 0) {

	xdiff1 = xDiffStore;	// restore these values
	ydiff1 = yDiffStore;

	yLimit = ydiff3;
	if ((float)(xdiff1) / (float)(ydiff1) < (float)(xdiff3) / (float)(ydiff3)) {  // swap x and y if one slope is creater than the other
		xdiff1 = xdiff3; xdiff3 = xDiffStore;
		ydiff1 = ydiff3; ydiff3 = yDiffStore;
	}

	j = 0;
	if (Y2 >= WINDOW.Y2) {j = Y2 - WINDOW.Y2 + 1;} 	// ensures that it doesn't draw below bottom of screen
	if (Y2 - yLimit < WINDOW.Y1) {yLimit = Y2 + WINDOW.Y1;}		// THIS MIGHT CAUSE PROBLEMS

	while (j < yLimit) {
		i1 = X2 - j * xdiff1 / ydiff1;	// calculate start and endpoints of line segment
		i2 = X2 - j * xdiff3 / ydiff3;
		if (i1 < WINDOW.X1) {i1 = WINDOW.Y1;}
		if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * (Y2 - j) + i1;
		drawMemEnd = WINDOW_WIDTH * (Y2 - j) + i2;
		while (drawMemStart < drawMemEnd) {
			pixels[drawMemStart] = color;
			drawMemStart++;
		}
		j++;
	}

	}
}*/

void drawTriangle(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x1 = drawInformation.arguments[0] + WINDOW.X1;
	int y1 = drawInformation.arguments[1] + WINDOW.Y1;
	int x2 = drawInformation.arguments[2] + WINDOW.X1;
	int y2 = drawInformation.arguments[3] + WINDOW.Y1;
	int x3 = drawInformation.arguments[4] + WINDOW.X1;
	int y3 = drawInformation.arguments[5] + WINDOW.Y1;
	int color = drawInformation.arguments[6];

	int X1, Y1, X2, Y2, X3, Y3;
	//X1 = 0;
	//Assign the uppercase variables according to the y-values: Y1 < Y2 < Y3
	if (y2 > y1) {
		if (y2 > y3) {
			X2 = x2;
			Y2 = y2;
			if (y3 > y1) {X3 = x3; Y3 = y3; X1 = x1; Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x3;Y1 = y3;}
		}
		else {
			X2 = x3;
			Y2 = y3;
			if (y2 > y1) {X3 = x2;Y3 = y2;	X1 = x1;Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x2;Y1 = y2;}
		}
	}
	else {
		if (y1 > y3) {
			X2 = x1;
			Y2 = y1;
			if (y3 > y2) {X3 = x3;Y3 = y3;X1 = x2;Y1 = y2;}
			else {X3 = x2;Y3 = y2;X1 = x3;Y1 = y3;}
		}
		else {
			X2 = x3;
			Y2 = y3;
			if (y2 > y1) {X3 = x2;	Y3 = y2;X1 = x1;Y1 = y1;}
			else {X3 = x1;Y3 = y1;X1 = x2;Y1 = y2;}
		}
	}


	int xdiff1 = X2 - X1; int ydiff1 = Y2 - Y1; //Line 1: (X1,Y1) -> (X2,Y2)
	int xdiff2 = X3 - X1; int ydiff2 = Y3 - Y1; //Line 2: (X1,Y1) -> (X3,Y3)
	int xdiff3 = X2 - X3; int ydiff3 = Y2 - Y3; //Line 3: (X2,Y2) -> (X3,Y3)
	int xDiffStore = xdiff1; // incase values are changed, store them before executing
	int yDiffStore = ydiff1;

	int yLimit = ydiff2;
	int j = 0;
	int i1, i2, drawMemStart, drawMemEnd; //drawing pointers for drawing horizontal segments

	if (ydiff2 != 0) {
        int yLimit = Y3-Y1;

		if (yLimit + Y1 >= WINDOW_HEIGHT) { yLimit = WINDOW_HEIGHT - Y1 - 1; } //Prevent screen overflow
		if (Y1 < 0) { j = -Y1; }

		while (j <= yLimit) {
			i1 = j * xdiff1 / ydiff1 + X1;	// calculate start and endpoints of line segment
			i2 = j * xdiff2 / ydiff2 + X1;

            if (i2 < i1) {
                int temp = i1;
                i1 = i2;
                i2 = temp;
            }

			if (i1 < 0) {i1 = 0;}
			if (i2 >= WINDOW_WIDTH) {i2 = WINDOW_WIDTH - 1;}
			drawMemStart = WINDOW_WIDTH * (j + Y1) + i1;
			drawMemEnd = WINDOW_WIDTH * (j + Y1) + i2;
			while (drawMemStart < drawMemEnd) {
				pixels[drawMemStart] = color;
				drawMemStart++;
			}
			j++;
		}
	}
	if (ydiff3 != 0) {
        yLimit = Y2-Y1;

		if (yLimit + Y1 >= WINDOW_HEIGHT) { yLimit = WINDOW_HEIGHT - Y1 - 1; } //Prevent screen overflow
		if (Y1 < 0) { j = -Y1; }

        while (j < yLimit) {
            i1 = j * xdiff1 / ydiff1 + X1;	// calculate start and endpoints of line segment
			i2 = (j - ydiff2) * xdiff3 / ydiff3 + X3;

            if (i2 < i1) {
                int temp = i1;
                i1 = i2;
                i2 = temp;
            }

			if (i1 < 0) {i1 = 0;}
			if (i2 >= WINDOW_WIDTH) {i2 = WINDOW_WIDTH - 1;}
			drawMemStart = WINDOW_WIDTH * (j + Y1) + i1;
			drawMemEnd = WINDOW_WIDTH * (j + Y1) + i2;
			while (drawMemStart < drawMemEnd) {
				pixels[drawMemStart] = color;
				drawMemStart++;
			}
			j++;
        }
	}
}

//void copyBufferSection( screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height ) {

void copyBufferSection(drawData drawInformation) {
	int xSource = drawInformation.arguments[0];
	int ySource = drawInformation.arguments[1];
	int xTarget = drawInformation.arguments[2];
	int yTarget = drawInformation.arguments[3];
	int width = drawInformation.arguments[4];
	int height = drawInformation.arguments[5];

	screenData * sourceBuffer = (screenData*) (drawInformation.dataPointer);
	screenData * targetBuffer = drawInformation.screen;

	if ( xSource < 0 ) {
		xTarget -= xSource;
		width += xSource;
		xSource = 0;
	}
	if ( xTarget < 0 ) {
		xSource -= xTarget;
		width += xTarget;
		xTarget = 0;
	}
	if ( ySource < 0 ) {
		yTarget -= ySource;
		height += ySource;
		ySource = 0;
	}
	if ( yTarget < 0 ) {
		ySource -= yTarget;
		height += yTarget;
		yTarget = 0;
	}

	if ( (targetBuffer -> windowWidth) - xTarget < width ) {
		width = (targetBuffer -> windowWidth) - xTarget;
	}
	if ( (sourceBuffer -> windowWidth) - xSource < width ) {
		width = (sourceBuffer -> windowWidth) - xSource;
	}

	if ( (targetBuffer -> windowHeight) - yTarget < height ) {
		height = (targetBuffer -> windowHeight) - yTarget;
	}
	if ( (sourceBuffer -> windowHeight) - ySource < height ) {
		height = (sourceBuffer -> windowHeight) - ySource;
	}

	int sourceStart = ySource * ( sourceBuffer -> windowWidth ) + xSource;
	int targetStart = yTarget * ( targetBuffer -> windowWidth ) + xTarget;

	int i;
	for ( i = 0; i < height; i++ ) {
		memcpy( (targetBuffer -> screen) + targetStart, (sourceBuffer -> screen) + sourceStart, width * 4 );
		targetStart += targetBuffer -> windowWidth;
		sourceStart += sourceBuffer -> windowWidth;
	}
}

void copyBufferScaled(drawData drawInformation) {
	screenData * sourceBuffer = (screenData*) (drawInformation.dataPointer);
	screenData * targetBuffer = drawInformation.screen;

	int xTarget = drawInformation.arguments[0];
	int yTarget = drawInformation.arguments[1];
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];

	int sourceHeight =  sourceBuffer -> windowHeight;
	int sourceWidth =  sourceBuffer -> windowWidth;
	int targetHeight =  targetBuffer -> windowHeight;
	int targetWidth =  targetBuffer -> windowWidth;

	int sourceAddress;
	uint32_t * pixelsSource = sourceBuffer -> screen;
	uint32_t * pixelsTarget = targetBuffer -> screen;

	int * targetWidths = new int[width];
	int i, j;
	for ( i = 0; i < width; i++ ) {
		targetWidths[i] = i * sourceWidth / width;
	}

	for ( j = 0; j < height; j++ ) {
		int targetAddress = ( yTarget + j )* targetWidth + xTarget;
		int sourceAddress = ( j * sourceHeight / height ) * sourceWidth;
		for ( i = 0; i < width; i++ ) {
			pixelsTarget[ targetAddress + i ] = pixelsSource[ sourceAddress + targetWidths[i] ];
		}
	}

	delete targetWidths;
}

void copyBufferSectionWithAlpha(drawData drawInformation) {
	int xSource = drawInformation.arguments[0];
	int ySource = drawInformation.arguments[1];
	int xTarget = drawInformation.arguments[2];
	int yTarget = drawInformation.arguments[3];
	int width = drawInformation.arguments[4];
	int height = drawInformation.arguments[5];

	screenData * sourceBuffer = (screenData*) (drawInformation.dataPointer);
	screenData * targetBuffer = drawInformation.screen;

	if ( xSource < 0 ) {
		xTarget -= xSource;
		width += xSource;
		xSource = 0;
	}
	if ( xTarget < 0 ) {
		xSource -= xTarget;
		width += xTarget;
		xTarget = 0;
	}
	if ( ySource < 0 ) {
		yTarget -= ySource;
		height += ySource;
		ySource = 0;
	}
	if ( yTarget < 0 ) {
		ySource -= yTarget;
		height += yTarget;
		yTarget = 0;
	}

	if ( (targetBuffer -> windowWidth) - xTarget < width ) {
		width = (targetBuffer -> windowWidth) - xTarget;
	}
	if ( (sourceBuffer -> windowWidth) - xSource < width ) {
		width = (sourceBuffer -> windowWidth) - xSource;
	}

	if ( (targetBuffer -> windowHeight) - yTarget < height ) {
		height = (targetBuffer -> windowHeight) - yTarget;
	}
	if ( (sourceBuffer -> windowHeight) - ySource < height ) {
		height = (sourceBuffer -> windowHeight) - ySource;
	}

	int sourceStart = ySource * ( sourceBuffer -> windowWidth ) + xSource;
	int targetStart = yTarget * ( targetBuffer -> windowWidth ) + xTarget;

	uint32_t * sourcePixels = (uint32_t *)(sourceBuffer -> screen);
	uint32_t * targetPixels = (uint32_t *)(targetBuffer -> screen);

	int i,j;
	for ( i = 0; i < height; i++ ) {
		for (j = 0; j < width; j++) {
			uint32_t sourcePix = sourcePixels[ sourceStart + j ];
			uint32_t targetPix = targetPixels[ targetStart + j ];

			int alpha = ( sourcePix >> 24 ) & 0xFF;
			int xalpha = 256 - alpha;
			int red = ( ( ( sourcePix & 0xFF0000 ) * alpha + ( targetPix & 0xFF0000 ) * xalpha ) >> 8 ) & 0xFF0000;
			int green = ( ( ( sourcePix & 0xFF00 ) * alpha + ( targetPix & 0xFF00 ) * xalpha ) >> 8 ) & 0xFF00;
			int blue = ( ( ( sourcePix & 0xFF ) * alpha + ( targetPix & 0xFF ) * xalpha ) >> 8 ) & 0xFF;

			targetPixels[ targetStart + j ] = red + green + blue;
		}

		targetStart += targetBuffer -> windowWidth;
		sourceStart += sourceBuffer -> windowWidth;
	}
}

void copyBufferScaledWithAlpha(drawData drawInformation) {
	screenData * sourceBuffer = (screenData*) (drawInformation.dataPointer);
	screenData * targetBuffer = drawInformation.screen;

	int xTarget = drawInformation.arguments[0];
	int yTarget = drawInformation.arguments[1];
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];

	int sourceHeight =  sourceBuffer -> windowHeight;
	int sourceWidth =  sourceBuffer -> windowWidth;
	int targetHeight =  targetBuffer -> windowHeight;
	int targetWidth =  targetBuffer -> windowWidth;

	int sourceAddress;
	uint32_t * pixelsSource = sourceBuffer -> screen;
	uint32_t * pixelsTarget = targetBuffer -> screen;

	int * targetWidths = new int[width];
	int i, j;
	for ( i = 0; i < width; i++ ) {
		targetWidths[i] = i * sourceWidth / width;
	}

	for ( j = 0; j < height; j++ ) {
		int targetAddress = ( yTarget + j )* targetWidth + xTarget;
		int sourceAddress = ( j * sourceHeight / height ) * sourceWidth;
		for ( i = 0; i < width; i++ ) {
			uint32_t sourcePix = pixelsSource[ sourceAddress + targetWidths[i] ];
			uint32_t targetPix = pixelsTarget[ targetAddress + i ];

			int alpha = ( sourcePix >> 24 ) & 0xFF;
			int xalpha = 256 - alpha;
			int red = ( ( ( sourcePix & 0xFF0000 ) * alpha + ( targetPix & 0xFF0000 ) * xalpha ) >> 8 ) & 0xFF0000;
			int green = ( ( ( sourcePix & 0xFF00 ) * alpha + ( targetPix & 0xFF00 ) * xalpha ) >> 8 ) & 0xFF00;
			int blue = ( ( ( sourcePix & 0xFF ) * alpha + ( targetPix & 0xFF ) * xalpha ) >> 8 ) & 0xFF;

			pixelsTarget[ targetAddress + i ] = red + green + blue;
		}
	}

	delete targetWidths;
}










