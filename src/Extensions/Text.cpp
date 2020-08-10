#include "GraphicsLibrary/Extensions/Text.h"

void drawTextRing(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;		// get screen information
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.x;		// get variables
	int y = drawInformation.arguments[1] + WINDOW.y;
	int rOut = drawInformation.arguments[2];
	int rIn = drawInformation.arguments[3];
	int color = drawInformation.arguments[4];
	int mask = drawInformation.arguments[5];

	int drawMemStart, drawMemEnd, drawMemStartIn, drawMemEndIn;
	float rOutSquared = (float)(rOut * rOut);
	float rInSquared = (float)(rIn * rIn);
	float rCutOff = (float)(rOut) * sqrt(2) / 2 + 1;

	bool top = !(mask & 1);
	bool bot = !(mask & 2);
	bool left = !(mask & 4);
	bool right = !(mask & 8);

	int j, i1, i2, yOutMax;
	if (mask & 1) { j = 0; }
	else { j = -rCutOff; }
	if (mask & 2) { yOutMax = 0; }
	else { yOutMax = rCutOff; }

	if (y - rOut < WINDOW.Y1) {j = -y + WINDOW.Y1;}
	if (y + yOutMax >= WINDOW.Y2) {yOutMax = WINDOW.Y2 - y - 1;}
	while (j <= yOutMax) {
		i1 = (int)(round(sqrt(rOutSquared - (float)(j * j)))) - 1;
		if (right) { i2 = i1 + x; } else { i2 = x; }
		if (left) { i1 = x - i1; } else { i1 = x; }
		if (i1 < WINDOW.X1) {i1 = WINDOW.X1;}
		if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * (j + y) + i1;
		drawMemEnd = WINDOW_WIDTH * (j + y) + i2;

		if ( j > rIn || j < -rIn ) {
			while (drawMemStart <= drawMemEnd) {
				pixels[drawMemStart] = color;
				drawMemStart++;
			}
		}
		else {
			i1 = (int)(round(sqrt(rInSquared - (float)(j * j))));
			i2 = i1 + x;
			i1 = x - i1;
			drawMemStartIn = WINDOW_WIDTH * (j + y) + i1;
			drawMemEndIn = WINDOW_WIDTH * (j + y) + i2;

			if (left) {while (drawMemStart <= drawMemStartIn) {
				pixels[drawMemStart] = color;
				drawMemStart++;
			}}
			if (right) {while (drawMemEndIn <= drawMemEnd) {
				pixels[drawMemEndIn] = color;
				drawMemEndIn++;
			}}
		}
		j++;
	}

	int i, j1, j2, xOutMax;
	if (mask & 4) { i = 0; }
	else { i = -rCutOff; }
	if (mask & 8) { xOutMax = 0; }
	else { xOutMax = rCutOff; }

	if (x - rOut < WINDOW.X1) {i = -x + WINDOW.X1;}
	if (x + xOutMax >= WINDOW.X2) {yOutMax = WINDOW.X2 - x - 1;}
	if (true) {
		while (i <= xOutMax) {
			j1 = (int)(round(sqrt(rOutSquared - (float)(i * i)))) - 1;
			if (bot) { j2 = j1 + y; } else { j2 = y; }
			if (top) { j1 = y - j1; } else { j1 = y; }
			if (j1 < WINDOW.X1) {j1 = WINDOW.X1;}
			if (j2 >= WINDOW.X2) {j2 = WINDOW.X2 - 1;}
			drawMemStart = WINDOW_WIDTH * j1 + (i + x);
			drawMemEnd = WINDOW_WIDTH * j2 + (i + x);

			if ( i > rIn || i < -rIn ) {
				while (drawMemStart <= drawMemEnd) {
					pixels[drawMemStart] = color;
					drawMemStart += WINDOW_WIDTH;
				}
			}
			else {
				j1 = (int)(round(sqrt(rInSquared - (float)(i * i))));
				j2 = j1 + y;
				j1 = y - j1;
				drawMemStartIn = WINDOW_WIDTH * j1 + (i + x);
				drawMemEndIn = WINDOW_WIDTH * j2 + (i + x);

				if (top) {while (drawMemStart <= drawMemStartIn) {
					pixels[drawMemStart] = color;
					drawMemStart += WINDOW_WIDTH;
				}}
				if (bot) {while (drawMemEndIn <= drawMemEnd) {
					pixels[drawMemEndIn] = color;
					drawMemEndIn += WINDOW_WIDTH;
				}}
			}
			i++;
		}
	}
}

drawData * addTextRing(int x, int y, int rOut, int rIn, int color, int mask, drawData * window) {
	int arguments[6] = { x, y, rOut, rIn, color, mask };
	return createWindowObject( drawTextRing, 5, arguments, NULL, 6, window );
}

void textCircle(drawData drawInformation) {
	uint32_t * pixels = drawInformation.screen -> screen;		// get screen information
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.x;		// get variables
	int y = drawInformation.arguments[1] + WINDOW.y;
	int rOut = drawInformation.arguments[2];
	int rIn = drawInformation.arguments[3];
	int color = drawInformation.arguments[4];
	int mask = drawInformation.arguments[5];

	int drawMemStart, drawMemEnd, drawMemStartIn, drawMemEndIn;
	float rOutSquared = (float)(rOut * rOut);
	float rInSquared = (float)(rIn * rIn);
	float rCutOff = (float)(rOut) * sqrt(2) / 2 + 1;

	int j, i1, i2, yOutMax;
	if (mask & 1) {
		j = 0;
		if (y < WINDOW.Y1) {j = -y + WINDOW.Y1;}
	}
	else {
		j = -rCutOff;
		if (y - rOut < WINDOW.Y1) {j = -y + WINDOW.Y1;}
	}
	if (mask & 2) {
		yOutMax = 0;
		if (y >= WINDOW.Y2) {yOutMax = WINDOW.Y2 - y - 1;}
	}
	else {
		yOutMax = rCutOff;
		if (y + yOutMax >= WINDOW.Y2) {yOutMax = WINDOW.Y2 - y - 1;}
	}
	while (j <= yOutMax) {
		i1 = (int)(round(sqrt(rOutSquared - (float)(j * j))));
		i2 = i1 + x;
		i1 = x - i1;
		if (i1 < WINDOW.X1) {i1 = WINDOW.X1;}
		if (i2 >= WINDOW.X2) {i2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * (j + y) + i1;
		drawMemEnd = WINDOW_WIDTH * (j + y) + i2;
		while (drawMemStart <= drawMemEnd) {
			pixels[drawMemStart] = color;
			drawMemStart++;
		}
		j++;
	}

	int i, j1, j2, xOutMax;
	if (mask & 1) {
		i = 0;
		if (x < WINDOW.X1) {i = -x + WINDOW.X1;}
	}
	else {
		i = -rCutOff;
		if (x - rOut < WINDOW.X1) {i = -x + WINDOW.X1;}
	}
	if (mask & 2) {
		xOutMax = 0;
		if (x >= WINDOW.X2) {yOutMax = WINDOW.X2 - x - 1;}
	}
	else {
		xOutMax = rCutOff;
		if (x + xOutMax >= WINDOW.X2) {yOutMax = WINDOW.X2 - x - 1;}
	}
	while (i <= xOutMax) {
		j1 = (int)(round(sqrt(rOutSquared - (float)(i * i))));
		j2 = j1 + y;
		j1 = y - j1;
		if (j1 < WINDOW.X1) {j1 = WINDOW.X1;}
		if (j2 >= WINDOW.X2) {j2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * j1 + (i + x);
		drawMemEnd = WINDOW_WIDTH * j2 + (i + x);
		while (drawMemStart <= drawMemEnd) {
			pixels[drawMemStart] = 0xFF0000;
			drawMemStart += WINDOW_WIDTH;
		}
		i++;
	}
}

void drawTextHShear( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;		// get screen information
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.x;
	int y = drawInformation.arguments[1] + WINDOW.y;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int color = drawInformation.arguments[4];
	int size = drawInformation.arguments[5];

	int x1, x2, drawMemStart, drawMemEnd;
	float slope = (float) width / (float) height;

	int j = 0;
	int yMax = height;
	if (y < WINDOW.Y1) {j = WINDOW.Y1 - y;}
	if (y + height >= WINDOW.Y2) {yMax = WINDOW.Y2 - y - 1; }

	while ( j <= yMax ) {
		x1 = x + round((float)(j)*slope);
		x2 = x1 + size;
		if (x1 < WINDOW.X1) {x1 = WINDOW.X1;}
		if (x2 >= WINDOW.X2) {x2 = WINDOW.X2 - 1;}
		drawMemStart = WINDOW_WIDTH * (y + j) + x1;
		drawMemEnd = WINDOW_WIDTH * (y+j) + x2;
		while (drawMemStart < drawMemEnd) {
			pixels[ drawMemStart ] = color;
			drawMemStart++;
		}
		j++;
	}
}
void drawTextVShear( drawData drawInformation ) {
	uint32_t * pixels = drawInformation.screen -> screen;		// get screen information
	int WINDOW_WIDTH = drawInformation.screen -> windowWidth;
	int WINDOW_HEIGHT = drawInformation.screen -> windowHeight;
	windowSection WINDOW = *(drawInformation.window);

	int x = drawInformation.arguments[0] + WINDOW.x;
	int y = drawInformation.arguments[1] + WINDOW.y;
	int width = drawInformation.arguments[2];
	int height = drawInformation.arguments[3];
	int color = drawInformation.arguments[4];
	int size = drawInformation.arguments[5];

	int y1, y2, drawMemStart, drawMemEnd;
	float slope = ((float)(height)) / ((float)(width));

	int i = 0;
	int xMax = width;
	if (x < WINDOW.X1) {i = WINDOW.X1 - x;}
	if (x + width >= WINDOW.X2) {xMax = WINDOW.X2 - x - 1; }

	while ( i <= xMax ) {
		y1 = y + round((float)(i)*slope);
		y2 = y1 + size;
		if (y1 < WINDOW.Y1) {y1 = WINDOW.Y1;}
		if (y2 >= WINDOW.Y2) {y2 = WINDOW.Y2 - 1;}
		drawMemStart = WINDOW_WIDTH * y1 + (x + i);
		drawMemEnd = WINDOW_WIDTH * y2 + (x + i);
		while (drawMemStart < drawMemEnd) {
			pixels[ drawMemStart ] = color;
			drawMemStart += WINDOW_WIDTH;
		}
		i++;
	}
}

drawData * addTextCircle(int x, int y, int rOut, int rIn, int color, int mask, drawData * window) {
	int arguments[6] = { x, y, rOut, rIn, color, mask };
	return createWindowObject( textCircle, 5, arguments, NULL, 5, window );
}

void drawChar( int x, int y, int color, int * charData, int Size, drawData stringData ) {
	drawData drawElem;
	drawElem.screen = stringData.screen;
	drawElem.window = stringData.window;
	drawElem.arguments[4] = color;

	float size = Size;
	int size1 = ceil((size) / 13);
	if (size < 15) {
		size1 = ceil((size + 1) / 13);
	}

	int n;
	int i = 0;
	int nHRectangles = charData[i];
	i++;
	for (n = 0; n < nHRectangles; n++) {
		drawElem.arguments[0] = x + round(charData[i] * size / 26);
		drawElem.arguments[1] = y + ceil(charData[i + 1] * size / 26);
		drawElem.arguments[2] = ceil(charData[i + 2] * size / 26);
		drawElem.arguments[3] = size1;
		drawRectangle( drawElem );
		i += 3;
	}

	int nVRectangles = charData[i];
	i++;
	for (n = 0; n < nVRectangles; n++) {
		drawElem.arguments[0] = x + round(charData[i] * size / 26);
		drawElem.arguments[1] = y + ceil(charData[i + 1] * size / 26);
		drawElem.arguments[2] = size1;
		drawElem.arguments[3] = ceil(charData[i + 2] * size / 26);
		drawRectangle( drawElem );
		i += 3;
	}

	int nCircles = charData[i];
	i++;
	for (n = 0; n < nCircles; n++) {
		drawElem.arguments[0] = x + round(charData[i] * size / 26);
		drawElem.arguments[1] = y + ceil(charData[i + 1] * size / 26);
		drawElem.arguments[2] = ceil(charData[i + 2] * size / 26);
		drawElem.arguments[3] = drawElem.arguments[2] - size1;
		drawElem.arguments[5] = charData[i + 3];
		drawTextRing( drawElem );
		i += 4;
	}

	int nHShears = charData[i];
	i++;
	for (n = 0; n < nHShears; n++) {
		drawElem.arguments[0] = x + round(charData[i] * size / 26);
		drawElem.arguments[1] = y + ceil(charData[i + 1] * size / 26);
		drawElem.arguments[2] = ceil(charData[i + 2] * size / 26);
		drawElem.arguments[3] = (charData[i + 3] * size / 26);
		drawElem.arguments[5] = size1;
		drawTextHShear( drawElem );
		i += 4;
	}
	int nVShears = charData[i];
	i++;
	for (n = 0; n < nVShears; n++) {
		drawElem.arguments[0] = x + round(charData[i] * size / 26);
		drawElem.arguments[1] = y + ceil(charData[i + 1] * size / 26);
		drawElem.arguments[2] = ceil(charData[i + 2] * size / 26);
		drawElem.arguments[3] = (charData[i + 3] * size / 26);
		drawElem.arguments[5] = size1;
		drawTextVShear( drawElem );
		i += 4;
	}
}


int charDataSpace[] = { 0,0,0,0,0 };
int charDataExclaim[] = { 0,
	1, 6,-26,20,
	1, 7,-2,2,0,
	0,0
};
int charDataDblQuote[] = {0,
	2, 3,-26,6, 9,-26,6,
	0,0,0
};

int charDataPoundSign[] = {2,  0,-18,14, 0,-10,14,
	0,0,
	2, 6,-24,-6,22, 12,-24,-6,22,
	0
};

int charDataDollarSign[] = {0,
	1, 6, -26, 26,
	4, 7,-8,6,4, 7,-8,6,1, 7,-18,6,8, 7,-18,6,2,
	0,0
};
int charDataPercentSign[] = {0,0,
	2, 4,-22,4,0, 10,-4,4,0,
	1, 12,-26,-12,26,
	0
};
int charDataAndSign[] = {0,
	3, 0,-10,3, 1,-21,2, 9,-21,2,
	5, 7,-7,7,1, 6,-21,5,2, 7,-10,7,10, 7,-19,4,5, 5,-19,4,9,
	1, 4,-15,8,15, 
	0
};
int charDataSingleQuote[] = {0,
	1, 6,-26,6,
	0,0,0
};
int charDataOpenParen[] = {0,0,
	1, 13,-13,13,8,
	0,0
};
int charDataCloseParen[] = {0,0,
	1, 1,-13,13,4,
	0,0
};
int charDataAsterisk[] = {0,
	1, 6,-26,13,
	0,0,
	2, 1,-24,12,7, 1,-17,12,-7
};
int charDataPlusSign[] = {1, 0,-14,14,
	1, 6,-20,14,
	0,0,0
};
int charDataComma[] = {0,
	1, 7,-2,2,
	2, 7,-2,2,0, 5,0,4,5,
	0,0
};
int charDataDash[] = {1, 0,-14,14,
	0,0,0,0
};
int charDataPeriod[] = {0,0,
	1, 7,-2,2,0,
	0,0
};
int charDataSlash[] = { 0,0,0,
	1, 12,-26,-12,26,
	0
};
int charData0[] = { 0,
	2, 0,-19,12, 12,-19,12,
	2, 7,-7,7,1, 7,-19,7,2,
	0,0
};
int charData1[] = { 1, 0, -2, 14,
	1, 6, -26, 26,
	0,
	1, 6, -26, -6, 6,
	0
};
int charData2[] = { 1, 0, -2, 14,
	2, 12,-19,3, 0,-4,2,
	3, 7,-19,7,2, 7,-4,7,10, 7,-16,7,5,
	0,0
};
int charData3[] = { 1, 6,-14,2,
	0,
	4, 7,-7,7,4, 7,-7,7,9, 7,-19,7,4, 7,-19,7,10,
	0, 0
};
int charData4[] = { 1, 0,-14,14,
	2, 0, -26, 14, 12,-26,26,
	0, 0, 0
};
int charData5[] = { 2, 0,-26,14, 0,-16,6,
	1, 0,-26,12,
	2, 6,-8,8,4, 6,-6,6,9,
	0, 0
};
int charData6[] = { 0,
	1, 0,-19,12,
	2, 7,-7,7,0, 7,-19,7,2,
	0, 0
};
int charData7[] = { 1, 0,-26,12,
	0, 0,
	1, 12,-26,-12,26,
	0
};
int charData8[] = { 0, 0,
	2, 7,-7,7,0, 7,-19,7,0,
	0, 0
};
int charData9[] = { 1, 7,-14,7,
	1, 12,-19,12,
	3, 7,-19,7,2, 7,-19,7,9, 7,-7,7,1,
	0, 0
};
int charDataColon[] = {0,0,
	2, 7,-2,2,0, 7,-13,2,0,
	0,0	
};
int charDataSemicolon[] = {0,
	1, 7,-2,2,
	3, 7,-2,2,0, 5,0,4,5, 7,-13,2,0,
	0,0
};
int charDataLessThan[] = {0,0,0,0,
	2, 0,-14,14,-6, 0,-14,14,6
};
int charDataEqualSign[] = {2,  0,-18,14, 0,-10,14,
	0,0,0,0
};
int charDataGreaterThan[] = {0,0,0,0,
	2, 0,-20,14,6, 0,-8,14,-6
};
int charDataQuestionMark[] = {0,
	2, 12,-19,3, 6,-10,4,
	4, 7,-19,7,2, 10,-16,4,5, 10,-10,4,10, 7,-2,2,0,
	0,0
};
int charDataAtSign[] = { 1, 7,-2,3,
	3, 0,-11,4, 12,-11,1, 8,-12,6,
	5, 7,-7,7,9, 7,-11,7,2, 7,-9,3,0, 10,-10,4,5, 10,-4,4,5,
	0,0
};
int charDataA[] = { 1, 3,-8,8,
	0,0,
	2, 6,-26,-6,26, 6,-26,6,26,
	0
};
int charDataB[] = { 3, 0,-14,7, 2,-26,5, 2,-2,5,
	1, 0,-26,26,
	2, 7,-7,7,4, 7,-19,7,4,
	0, 0
};
int charDataC[] = { 0,
	1, 0,-19,12,
	2, 7,-7,7,1, 7,-19,7,2,
	0,0
};
int charDataD[] = { 2, 2,-26,3, 2,-2,3,
	2, 0,-26,26, 12,-17,8,
	2, 5,-9,9,5, 5,-17,9,6,
	0, 0
};
int charDataE[] = { 3, 0,-14,10, 0,-26,14, 0,-2,14,
	1, 0,-26,26,
	0,0,0
};
int charDataF[] = { 2, 0,-14,10, 0,-26,14,
	1, 0,-26,26,
	0,0,0
};
int charDataG[] = { 1, 7,-14,7,
	2, 0,-19,12, 12,-12,5,
	2, 7,-7,7,1, 7,-19,7,2,
	0,0
};
int charDataH[] = { 1, 0,-14,14,
	2, 0,-26,26, 12,-26,26,
	0,0,0
};
int charDataI[] = { 2, 0,-2,14, 0,-26,14,
	1, 6,-26,26,
	0,0,0
};
int charDataJ[] = { 1, 0,-26,14,
	1, 10,-26,20,
	1, 6,-6,6,1,
	0,0
};
int charDataK[] = { 0,
	1, 0,-26,26,
	0,
	2, 12,-26,-12,14, 0,-14,12,14,
	0//2, 2,-14,12,-12, 2,-14,12,12
};
int charDataL[] = { 1, 0,-2,14,
	1, 0,-26,26,
	0,0,0
};
int charDataM[] = { 0,
	2, 0,-26,26, 12,-26,26,
	0,
	2, 0,-26,6,20, 12,-26,-6,20,
	0
};
int charDataN[] = { 0,
	2, 0,-26,26, 12,-26,26,
	0,
	1, 0,-26,12,26,
	0
};
int charDataO[] = { 0,
	2, 0,-19,12, 12,-19,12,
	2, 7,-7,7,1, 7,-19,7,2,
	0,0
};
int charDataP[] = { 2, 0,-14,7, 2,-26,5,
	1, 0,-26,26,
	1, 7,-19,7,4,
	0, 0
};
int charDataQ[] = { 0,
	2, 0,-19,12, 12,-19,12,
	2, 7,-7,7,1, 7,-19,7,2,
	1, 5,-9,7,9,
	0
};
int charDataR[] = { 2, 0,-14,7, 2,-26,5,
	1, 0,-26,26,
	1, 7,-19,7,4,
	1, 2,-12,10,12,
	0
};
int charDataS[] = { 0,0,
	4, 7,-7,7,4, 7,-7,7,1, 7,-19,7,8, 7,-19,7,2,
	0,0,
};
int charDataT[] = { 1, 0,-26,14,
	1, 6,-26,26,
	0,0,0
};
int charDataU[] = { 0,
	2, 0,-26,19, 12,-26,19,
	1, 7,-7,7,1,
	0,0
};
int charDataV[] = { 0,0,0,
	2, 0,-26,6,26, 12,-26,-6,26,
	0
};
int charDataW[] = { 0,
	2, 0,-26,26, 12,-26,26,
	0,
	2, 6,-20,-6,20, 6,-20,6,20,
	0
};
int charDataX[] = { 0,0,0,
	2, 0,-26,12,26, 12,-26,-12,26,
	0
};
int charDataY[] = { 0,
	1, 6,-12,12,
	0,
	2, 0,-26,6,14, 12,-26,-6,14,
	0
};
int charDataZ[] = { 2, 0,-26,12, 2,-2,12,
	0,0,
	1, 12,-26,-12,26,
	0
};
int charDataOpenBrac[] = { 2, 0,-2,14, 0,-26,14,
	1, 0,-26,26,
	0,0,0
};
int charDataBackSlash[] = {0,0,0,
	1, 12,-26,-12,26,
	0
};
int charDataCloseBrac[] = { 2, 0,-2,14, 0,-26,14,
	1, 12,-26,26,
	0,0,0
};
int charDataCarrot[] = { 0,0,0,
	2, 6,-26,-4,6, 6,-26,4,6,
	0
};
int charDataUnderscore[] = { 1, 0,-2,14,
	0,0,0,0
};
int charDataAccent[] = { 0,0,0,
	1, 6,-26,4,6,
	0
};
int charDataa[] = { 0,
	1, 12,-14,14,
	1, 7,-7,7,0,
	0
};
int charDatab[] = { 0,
	1, 0,-26,26,
	1, 7,-7,7,0,
	0
};
int charDatac[] = { 2, 7,-14,7, 7,-2,7,
	0,
	1, 7,-7,7,8,
	0
};
int charDatad[] = { 0,
	1, 12,-26,26,
	1, 7,-7,7,0,
	0
};
int charDatae[] = { 2, 0,-8,14, 7,-2,7,
	0,
	2, 7,-7,7,2, 7,-7,7,9,
	0
};
int charDataf[] = { 1, 0,-14,14,
	1, 4,-21,21,
	1, 9,-21,5,2,
	0
};
int charDatag[] = { 1, 0,5,8, 
	1, 12,-14,14,
	2, 7,-7,7,0, 7,0,7,5,
	0
};
int charDatah[] = { 0, 
	2, 0,-26,26, 12,-7,7,
	1, 7,-7,7,2,
	0
};
int charDatai[] = { 2, 2,-2,10, 4,-14,4,
	1, 6,-14,14,
	1, 7,-18,2,0,
	0
};
int charDataj[] = { 0,
	1, 10,-14,14,
	2, 6,0,6,1, 11,-18,2,0,
	0
};
int charDatak[] = { 0,
	1, 0,-26,26,
	0,0,
	2, 2,-8,12,6, 2,-8,12,-6,
};
int charDatal[] = { 2, 2,-2,10, 4,-26,4,
	1, 6,-26,26,
	0,0,0
};
int charDatam[] = { 0,
	3, 0,-14,14, 6,-10,10, 12,-10,10,
	2, 4,-10,4,2, 10,-10,4,2,
	0,0
};
int charDatan[] = { 0,
	2, 0,-14,14, 12,-7,7,
	1, 7,-7,7,2,
	0,0
};
int charDatao[] = { 0,0,
	1, 7,-7,7,0,
	0,0
};
int charDatap[] = { 0,
	1, 0,-14,21,
	1, 7,-7,7,0,
	0,0
};
int charDataq[] = { 0,
	1, 12,-14,21,
	1, 7,-7,7,0,
	0,0
};
int charDatar[] = { 0,
	1, 0,-14,14,
	1, 7,-7,7,2,
	0
};
int charDatas[] = { 3, 5,-14,8, 5,-8,4, 1,-2,8,
	0,
	2, 5,-10,4,8, 9,-4,4,4,
	0
};
int charDatat[] = { 1, 0,-14,14,
	1, 6,-26,26,
	0,0,0
};
int charDatau[] = { 0,
	2, 0,-14,7, 12,-14,14,
	1, 7,-7,7,1,
	0,0
};
int charDatav[] = { 0,0,0,
	2, 0,-14,6,14, 12,-14,-6,14,
	0
};
int charDataw[] = { 0,0,0,
	4, 0,-14,3,14, 6,-14,-3,14, 6,-14,3,14, 12,-14,-3,14,
	0
};
int charDatax[] = { 0,0,0,
	2, 0,-14,12,14, 12,-14,-12,14,
	0
};
int charDatay[] = { 1, 0,5,8,
	2, 0,-14,7, 12,-14,14,
	2, 7,-7,7,1, 7,0,7,5,
	0,0

};
int charDataz[] = { 2, 0,-14,12, 2,-2,12,
	0,0,
	1, 12,-14,-12,14,
	0
};
int charDataOpenBrace[] = { 0,0,
	4, 1,-19,7,5, 1,-7,7,6, 13,-19,7,10, 13,-7,7,9,
	0,0
};
int charDataDivider[] = { 0,
	1, 6,-26,26,
	0,0,0
};
int charDataClosedBrace[] = { 0,0,
	4, 1,-19,7,6, 1,-7,7,5, 13,-19,7,9, 13,-7,7,10,
	0,0
};
int charDataTilde[] = { 0,0,
	2, 4,-14,4,2, 10,-14,4,1,
	0,0
};


int * charDataList[] = { charDataSpace, charDataExclaim, charDataDblQuote, charDataPoundSign, charDataDollarSign, charDataPercentSign, charDataAndSign, charDataSingleQuote, charDataOpenParen, charDataCloseParen,
charDataAsterisk, charDataPlusSign, charDataComma, charDataDash, charDataPeriod, charDataSlash, charData0, charData1, charData2, charData3, charData4, charData5, charData6, charData7, charData8, charData9,
charDataColon, charDataSemicolon, charDataLessThan, charDataEqualSign, charDataGreaterThan, charDataQuestionMark, charDataAtSign, charDataA, charDataB, charDataC, charDataD, charDataE, charDataF, charDataG,
charDataH, charDataI, charDataJ, charDataK, charDataL, charDataM, charDataN, charDataO, charDataP, charDataQ, charDataR, charDataS, charDataT, charDataU, charDataV, charDataW, charDataX, charDataY, charDataZ,
charDataOpenBrac, charDataBackSlash, charDataCloseBrac, charDataCarrot, charDataUnderscore, charDataAccent, charDataa, charDatab, charDatac, charDatad, charDatae, charDataf, charDatag, charDatah, charDatai,
charDataj, charDatak, charDatal, charDatam, charDatan, charDatao, charDatap, charDataq, charDatar, charDatas, charDatat, charDatau, charDatav, charDataw, charDatax, charDatay, charDataz,
charDataOpenBrace, charDataDivider, charDataClosedBrace, charDataTilde
};

void drawString2( drawData drawInfo ) {
	int x = drawInfo.arguments[0];
	int y = drawInfo.arguments[1];
	int color = drawInfo.arguments[2];
	int size = drawInfo.arguments[3];

	int width = ceil( (float)(size) * 15 / 26 ) + 1;

	char * string = (char *)(drawInfo.dataPointer);

	int i;
	for ( i = 0; string[i] != 0; i++ ) {
		drawChar( x, y, color, charDataList[ string[i] - 32 ], size, drawInfo );
		x += width;
	}
}

drawData * addText2(char * string, int x, int y, int width, int size, int color, int quality, drawData * window) {
	int arguments[4] = { x, y, color, size };
	return createWindowObject( drawString2, 5, arguments, string, 6, window );
}
