
#include "GraphicsLibrary/Extensions/TrueType.h"
#include "GraphicsLibrary/Extensions/DataOperations.h"

char defaultFontSans[] = "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf";
char defaultFontSerif[] = "/usr/share/fonts/truetype/liberation/LiberationSerif-Regular.ttf";
char defaultFontMono[] = "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf";

u32 u4beToLE(u4be x) {
    return (u32)((x & 0xFF000000) >> 24) + ((x & 0xFF0000) >> 8) + ((x & 0xFF00) << 8) + ((x & 0xFF) << 24);
}

u16 u2beToLE(u2be x) {
    return (u16)(((x & 0xFF00) >> 8) + ((x & 0xFF) << 8));
}

i32 s4beToLE(s4be x) {
    return (i32)u4beToLE((u4be)x);
}

i16 s2beToLE(s2be x) {
    return (i16)u2beToLE((u2be)x);
}

void * getTtfTable(ttfRoot * t, const char name[4]) {
    u16 num_tables = u2beToLE(t -> OffsetTable.num_tables);

    u16 i = 0;
    TtfDirTableEntry * offsets = (TtfDirTableEntry *)((u8 *)t + 12);
    for (i = 0; i < num_tables; i++) {
        if (compareData((u8*)offsets[i].tag, (u8*)name, 4)) {
            u32 offset = u4beToLE(offsets[i].offset);
            return (void *)((u8*)t + offset);
        }
    }
    return NULL;
}

void deleteGlyphInfo(glyphInfo * info) {
    delete info -> flags;
    delete info -> points;
    delete info -> endpoints;
    delete info;
}

int getCharacterGlyphIndex(ttfRoot * t, int c) {
    ttfCmapHeader * cmapHeader = (ttfCmapHeader *)getTtfTable(t, "cmap");

    u16 numTables = u2beToLE(cmapHeader -> numTables);
    ttfCmapSubtableHeader * subtables = (ttfCmapSubtableHeader *)(cmapHeader + 1);

    int i;
    for (i = 0; i < numTables; i++) {
        u32 subtableOffset = u4beToLE(subtables[i].subtableOffset);

        u8 * subtable = (u8*)(cmapHeader) + subtableOffset;
        u16 format = u2beToLE(*(u16*)subtable);
        u16 length = u2beToLE(*(u16*)(subtable + 2));
        u16 language = u2beToLE(*(u16*)(subtable + 4));

        if (format == 4) {
            u16 segCount = u2beToLE(*(u16*)(subtable + 6)) / 2;
            u2be * endCode = (u2be *)(subtable + 14);
            u2be * startCode = (u2be *)(subtable + 16 +   2*segCount);
            s2be * idDelta   = (s2be *)(subtable + 16 + 2*2*segCount);
            u2be * idRangeOffset=(u2be*)(subtable+ 16 + 3*2*segCount);
            u2be * glyphIdArray=(u2be*)(subtable + 16 + 4*2*segCount);

            int j;
            for (j = 0; j < segCount; j++) {
                if ( u2beToLE(endCode[j]) >= c ) {
                    if ( u2beToLE(startCode[j]) > c) {
                        return 0;
                    }
                    int glyphId;
                    if (idRangeOffset[j] != 0) {
                        glyphId = *(u2beToLE(idRangeOffset[j])/2 + (c - u2beToLE(startCode[j])) + &idRangeOffset[j]);
                    }
                    else {
                        glyphId = c + s2beToLE(idDelta[j]);
                    }
                    return glyphId;
                }
            }
        }
    }

    return 0;
}

int getGlyphOffset(ttfRoot * t, int offset) {
    u16 indexToLocFormat = s2beToLE( ((ttfHead*)getTtfTable(t, "head")) -> index_to_loc_format );

    void * cmapHeaderRaw = getTtfTable(t, "loca");
    
    if (indexToLocFormat) {
        u4be * cmapHeader = (u4be *)cmapHeaderRaw;

        return (int)s4beToLE(cmapHeader[offset]);
    }
    else {
        u2be * cmapHeader = (u2be *)cmapHeaderRaw;

        return (int)s2beToLE(cmapHeader[offset]) * 2;
    }
}

void getGlyphAdvanceWidth(ttfRoot * t, int glyphIndex, glyphInfo * info) {
    ttfHhea * hhea = (ttfHhea *)getTtfTable(t, "hhea");
    ttfHMetric * hMetrics = (ttfHMetric *)getTtfTable(t, "hmtx");
    if (hhea == NULL || hMetrics == NULL) {
        return;
    }
    int nHmetrics = u2beToLE(hhea -> number_of_hmetrics);
    if (glyphIndex >= nHmetrics) {
        glyphIndex = nHmetrics - 1;
    }
    info -> advanceWidth = u2beToLE(hMetrics[glyphIndex].advanceWidth);
}


glyphInfo * getGlyphInfo(ttfRoot * t, int c) {

    int glyphIndex = getCharacterGlyphIndex(t, c);
    int glyphOffset = getGlyphOffset(t, glyphIndex);

    ttfGlyf * g = (ttfGlyf*)((u8*)getTtfTable(t, "glyf") + glyphOffset);
    if (g -> number_of_contours < 0) {
        return NULL;
    }

    glyphInfo * info = new glyphInfo;
    ttfHead * head = (ttfHead*)getTtfTable(t, "head");
    int w = s2beToLE(head -> x_max) - s2beToLE(head -> x_min);
    int h = s2beToLE(head -> y_max) - s2beToLE(head -> y_min);
    info -> x1 = s2beToLE(g -> x_min);
    info -> y1 = s2beToLE(g -> y_min);
    info -> x2 = s2beToLE(g -> x_max);
    info -> y2 = s2beToLE(g -> y_max);
    info -> X1 = s2beToLE(head -> x_min);
    info -> Y1 = s2beToLE(head -> y_min);
    info -> X2 = s2beToLE(head -> x_max);
    info -> Y2 = s2beToLE(head -> y_max);
    info -> glyphIndex = glyphIndex;

    i16 nContours = s2beToLE(g -> number_of_contours);
    info -> nContours = nContours;
    info -> endpoints = new int[nContours];

    u8 * p = (u8*)g + 10;
    u16 pointCount = 0;
    int i;
    for (i = 0; i < nContours; i++) {
        u16 endPoint = u2beToLE(*(u16*)(p + 2 * i));
        info -> endpoints[i] = endPoint;

        if (endPoint > pointCount) {
            pointCount = endPoint;
        }
    }
    pointCount++;

    p += nContours * 2;
    u16 instructionLength = u2beToLE(*(u16*)p);
    p += 2;

    p += instructionLength;
    
    u8 * flags = new u8[pointCount];
    int * points = new int[2 * pointCount];

    for (i = 0; i < pointCount; i++) {
        u8 flag = *p;
        flags[i] = flag;

        if (flag & 0x08) {
            p++;
            int repeat = (int)*p;
            int j = 0;
            for (j = 0; j < repeat; j++) {
                i++;
                flags[i] = flag;
            }
        }
        p++;
    }

    i16 lastX = 0;
    for (i = 0; i < pointCount; i++) {
        u8 flag = flags[i];

        i16 x;
        if (flag & 0x2) {
            x = (i16)(*(u8*)p);
            if (!(flag & 0x10)) {
                x = -x;
            }
            x += lastX;
            p++;
        }
        else {
            if (flag & 0x10) {
                x = lastX;
            }
            else {
                x = lastX + s2beToLE(*(i16*)p);
                p += 2;
            }
        }

        lastX = x;

        points[2 * i] = x - info -> x1;
    }

    i16 lastY = 0;
    for (i = 0; i < pointCount; i++) {

        u8 flag = flags[i];

        i16 y;
        if (flag & 0x4) {
            y = (i16)(*(u8*)p);
            if (!(flag & 0x20)) {
                y = -y;
            }
            y += lastY;
            p++;
        }
        else {
            if (flag & 0x20) {
                y = lastY;
            }
            else {
                y = lastY + s2beToLE(*(i16*)p);
                p += 2;
            }
        }

        lastY = y;
        points[2 * i + 1] = (info -> Y2) - y;
    }

    info -> flags = flags;
    info -> points = points;
    info -> nPoints = pointCount;
    info -> w = w;
    info -> h = h;
    getGlyphAdvanceWidth(t, glyphIndex, info);

    return info;
}

void getTrackingTable(ttfRoot * t) {
    ttfTrackTable * trak = (ttfTrackTable *)getTtfTable(t, "trak");
    if (trak == NULL) {
        return;
    }

    u16 horizOffset = u2beToLE(trak -> horizOffset);
    ttfTrackData * data = (ttfTrackData *)( ((u8*)trak) + (int)horizOffset );

    ttfTrackDataEntry * entry = (ttfTrackDataEntry *)( ((u8*)data) + sizeof(ttfTrackData) );

    int nTracks = (int)u2beToLE(data -> nTracks);
    int i;
    for ( i = 0; i < nTracks; i++ ) {
        entry++;
    }
}

void getHTMXTable(ttfRoot * t) {
    ttfTrackTable * trak = (ttfTrackTable *)getTtfTable(t, "trak");
}

screenData * downsizeCharacterBuffer(unsigned char * pixels, int w, int h, int downSize) {
	int W = w / downSize;
	int H = h / downSize;
	screenData * screen = createOffscreenBuffer(W, H);
	Uint32 * p = screen -> screen;

	int xCut1 = downSize / 3;
	int xCut2 = 2 * downSize / 3;

	if (downSize < 3) {
		int i, j;
		for (i = 0; i < W; i++) {
			for (j = 0; j < H; j++) {
				int I,J;
				int val = 0;

				for (I = 0; I < downSize; I++) {
					for (J = 0; J < downSize; J++) {
						val += (int)pixels[(downSize * j + J) * w + (downSize * i + I)];
					}
				}

				val /= (downSize * downSize);
				p[W * j + i] = (val << 16) + (val << 8) + val;
			}
		}
		return screen;
	}

	int i, j;
	for (i = 0; i < W; i++) {
		for (j = 0; j < H; j++) {
			int I,J;
			int r = 0; int b = 0; int g = 0;
			for (I = 0; I < xCut1; I++) {
				for (J = 0; J < downSize; J++) {
					r += (int)pixels[(downSize * j + J) * w + (downSize * i + I)];
				}
			}
			for (I = xCut1; I < xCut2; I++) {
				for (J = 0; J < downSize; J++) {
					g += (int)pixels[(downSize * j + J) * w + (downSize * i + I)];
				}
			}
			for (I = xCut2; I < downSize; I++) {
				for (J = 0; J < downSize; J++) {
					b += (int)pixels[(downSize * j + J) * w + (downSize * i + I)];
				}
			}
			r /= (downSize * (xCut1));
			g /= (downSize * (xCut2 - xCut1));
			b /= (downSize * (downSize - xCut2));
			r = r*r/255;
			g = g*g/255;
			b = b*b/255;
			
			p[W * j + i] = (r << 16) + (g << 8) + b;
		}
	}

	for (j = 0; j < H; j++) {
		int r,g,b;
		int vOld = p[W*j];
		int vNew = p[W*j + 1];

		int b0 = 255;
		int r1 = (vOld & 0xFF0000) >> 16;
		int g1 = (vOld & 0xFF00) >> 8;
		int b1 = vOld & 0xFF;
		int r2 = (vNew & 0xFF0000) >> 16;

		r = (b0 + r1 + g1) / 3;
		g = (r1 + g1 + b1) / 3;
		b = (g1 + b1 + r2) / 3;
		r = sqrt(r * 255);
        g = sqrt(g * 255);
        b = sqrt(b * 255);
		p[W * j] = (r << 16) + (g << 8) + b;

		for (i = 1; i < W - 1; i++) {
			vOld = vNew;
			vNew = p[W*j + 1 + i];

			b0 = b1;
			r1 = r2;
			g1 = (vOld & 0xFF00) >> 8;
			b1 = vOld & 0xFF;
			r2 = (vNew & 0xFF0000) >> 16;

			r = (b0 + r1 + g1) / 3;
			g = (r1 + g1 + b1) / 3;
			b = (g1 + b1 + r2) / 3;
			r = sqrt(r * 255);
			g = sqrt(g * 255);
			b = sqrt(b * 255);
			p[W * j + i] = (r << 16) + (g << 8) + b;
		}

		b0 = b1;
		r1 = r2;
		g1 = (vNew & 0xFF00) >> 8;
		b1 = vNew & 0xFF;
		r2 = 255;
		r = (b0 + r1 + g1) / 3;
		g = (r1 + g1 + b1) / 3;
		b = (g1 + b1 + r2) / 3;
		r = sqrt(r * 255);
        g = sqrt(g * 255);
        b = sqrt(b * 255);
		p[W * j + W - 1] = (r << 16) + (g << 8) + b;
	}

	return screen;
}

void drawTriangleFixBezier(float x1i, float y1i, float x2i, float y2i, float x3i, float y3i, unsigned char * pixels, int WINDOW_WIDTH, int WINDOW_HEIGHT) {

	int x1 = (int)x1i;
	int y1 = (int)y1i;
	int x2 = (int)x2i;
	int y2 = (int)y2i;
	int x3 = (int)x3i;
	int y3 = (int)y3i;

	int X1, Y1, X2, Y2, X3, Y3;
	
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

	float a = x3i - x1i;
	float b = x2i - x1i;
	float c = y3i - y1i;
	float d = y2i - y1i;
	float D = a*d-b*c;

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
			float y = j + Y1;
			float x = i1;
			while (drawMemStart < drawMemEnd) {
				float e = x - x1i;
				float f = y - y1i;
				float s = (d*e-b*f)/D;
				float t = (-c*e+a*f)/D;
				float tmp = s / 2 + t;
				if (tmp*tmp < t) {
					pixels[drawMemStart] += 1;
				}
				drawMemStart++;
				x += 1;
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
			float y = j + Y1;
			float x = i1;
			while (drawMemStart < drawMemEnd) {
				float e = x - x1i;
				float f = y - y1i;
				float s = (d*e-b*f)/D;
				float t = (-c*e+a*f)/D;
				float tmp = s / 2 + t;
				if (tmp*tmp < t) {
					pixels[drawMemStart] += 1;
				}
				drawMemStart++;
				x += 1;
			}
			j++;
        }
	}
}
void drawTriangleForPoly(float x1i, float y1i, float x2i, float y2i, float x3i, float y3i, unsigned char * pixels, int WINDOW_WIDTH, int WINDOW_HEIGHT) {

	int x1 = (int)x1i;
	int y1 = (int)y1i;
	int x2 = (int)x2i;
	int y2 = (int)y2i;
	int x3 = (int)x3i;
	int y3 = (int)y3i;

	int X1, Y1, X2, Y2, X3, Y3;
	
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
				pixels[drawMemStart] += 1;
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
				pixels[drawMemStart] += 1;
				drawMemStart++;
			}
			j++;
        }
	}
}
void copyBufferRGBShade(screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height) {

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

	Uint32 * s = (sourceBuffer -> screen) + sourceStart;
	Uint32 * t = (targetBuffer -> screen) + targetStart;
	int j;
	for ( j = 0; j < height; j++ ) {
		int i;
		for (i = 0; i < width; i++) {
			int val = s[i];
            if (val != 0xFFFFFF) {
                int val2 = t[i];
                int r = ((val & 0xFF0000) >> 16) * ((val2 & 0xFF0000) >> 16) / 255;
                int g = ((val & 0xFF00) >> 8) * ((val2 & 0xFF00) >> 8) / 255;
                int b = ((val & 0xFF)) * ((val2 & 0xFF)) / 255;
                t[i] = (r << 16) + (g << 8) + b;
            }
		}
		t += targetBuffer -> windowWidth;
		s += sourceBuffer -> windowWidth;
	}
}

void deleteFontCharacterCache(fontCharacterCache * cache) {
    delete cache -> screen;
    delete cache;
}

fontCharacterCache * drawTrueTypeCharacter(int c, trueTypeFontRenderer * renderer) {
    if (c >= 128) { return NULL; }

    fontCharacterCache * f = renderer -> cache[c];  
    if (f == NULL) {
        f = new fontCharacterCache;

        glyphInfo * info = renderer -> descriptor -> glyphInfoCache[c];
        if (info == NULL) {
            fileData * fontFileRaw = renderer -> descriptor -> fontFileRaw;
            ttfRoot * t = (ttfRoot *)(fontFileRaw -> buffer);
            info = getGlyphInfo(t, c);
            renderer -> descriptor -> glyphInfoCache[c] = info;
        }

        f -> screen = NULL;
        renderer -> cache[c] = f;
    }

    if (f -> screen != NULL) {
        return f;
    }

    int fontSize = renderer -> fontSize;
	int aliasScale = renderer -> aliasScale;
    
    glyphInfo * info = renderer -> descriptor -> glyphInfoCache[c];
	int * points = info -> points;
	int len = info -> nPoints;
	int glyphIndex = info -> glyphIndex;

	float scaleDown = ((float)info -> h) / (fontSize * aliasScale);
	float yOffset = floor( ((float)info -> Y2 - (float)info -> y2) / scaleDown / aliasScale) * aliasScale;
	int w = ceil( ((float)(info -> x2) - (float)(info -> x1)) / scaleDown ) + aliasScale;
	int h = ceil( ( ((float)info -> Y2 - (float)info -> y1) - yOffset) / scaleDown )+ aliasScale;

    unsigned char * screen = new unsigned char[w*h];
    memset(screen, 0, w*h);

    if (c <= 32) {
        info -> nContours = 0;
    }
	int j; int i;
	for (j = 0; j < info -> nContours; j++) {
		int * poly;
		u8 * flags;
		if (j == 0) {
			poly = points;
			len = info -> endpoints[j] + 1;
			flags = info -> flags;
		}
		else {
			poly = points + 2 * (info -> endpoints[j - 1] + 1);
			len = info -> endpoints[j] - info -> endpoints[j - 1];
			flags = info -> flags + info -> endpoints[j - 1] + 1;
		}

		int ptS[2];
		int * pt1 = NULL;
		int * bezRef = NULL;
		int * bezEnd = NULL;
		int * ptEnd = NULL;
		for (i = 0; i < len; i++) {
			if (flags[i] & 1) {
				pt1 = &poly[2 * i];
				ptEnd = pt1;
				i++;
				break;
			}
			else {
				bezEnd = &poly[2*i];
				bezRef = &poly[2*i];
			}
		}
		int phase = 0;

		for (i = i; i < len; i++) {
			if (flags[i] & 1) {
				if (phase > 0) {
					drawTriangleFixBezier(((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
											((float)poly[2*i])/scaleDown, ((float)poly[2*i+1])/scaleDown - yOffset,
											((float)bezRef[0])/scaleDown, ((float)bezRef[1])/scaleDown - yOffset, screen, w, h);
				}
				drawTriangleForPoly(0, 0, 
					((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
					((float)poly[2*i])/scaleDown, ((float)poly[2*i+1])/scaleDown - yOffset, screen, w, h);
				phase = 0;
				pt1 = &poly[2*i];
			}
			else {
				if (phase > 0) {
					int ptX = (bezRef[0] + poly[2*i]) / 2;
					int ptY = (bezRef[1] + poly[2*i + 1]) / 2;

					drawTriangleForPoly(0, 0, 
						((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
						((float)ptX)/scaleDown, ((float)ptY)/scaleDown - yOffset, screen, w, h);

					drawTriangleFixBezier(((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
						((float)ptX)/scaleDown, ((float)ptY)/scaleDown - yOffset,
						((float)bezRef[0])/scaleDown, ((float)bezRef[1])/scaleDown - yOffset, screen, w, h);

					// fix bezier curve
					ptS[0] = ptX;
					ptS[1] = ptY;
					phase = 1;
					pt1 = ptS;
				}
				else {
					phase++;
				}
				bezRef = &poly[2*i];
			}
		}

		if (phase > 0) {
			bezEnd = bezRef;
		}
		if (ptEnd != NULL && pt1 != NULL) {
			drawTriangleForPoly(0, 0, 
				((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
				((float)ptEnd[0])/scaleDown, ((float)ptEnd[1])/scaleDown - yOffset, screen, w, h);
			
			if (bezEnd != NULL && phase != 0) {
				drawTriangleFixBezier(((float)pt1[0])/scaleDown, ((float)pt1[1])/scaleDown - yOffset,
						((float)ptEnd[0])/scaleDown, ((float)ptEnd[1])/scaleDown - yOffset,
						((float)bezEnd[0])/scaleDown, ((float)bezEnd[1])/scaleDown - yOffset, screen, w, h);
			}
		}
	}

    int size = w*h;
    unsigned char * s = screen;
    for (i = 0; i < size; i++) {
		if (s[i] % 2) {
			s[i] = 0;
		}
		else {
			s[i] = 255;
		}
    }

    screenData * charScreen = downsizeCharacterBuffer(screen, w, h, aliasScale);
    delete screen;

	float downSize = ((float)aliasScale) * scaleDown;
	f -> screen = charScreen;
	f -> fontWidth = ( (float)(info -> x2) - (float)(info -> x1) ) / downSize;
	f -> fontHeight = ( (float)(info -> y2) - (float)(info -> y1) ) / downSize;
	f -> xBufferOffset = ( (float)(info -> x1) ) / downSize;
	f -> yBufferOffset = ( (float)(info -> Y2) - (float)(info -> y2) ) / downSize;
	f -> yBufferOffset = yOffset / aliasScale;
	f -> glyphIndex = glyphIndex;
    f -> advanceWidth = (float)(info -> advanceWidth) / downSize;

    f -> totalWidth = f -> fontWidth + f -> xBufferOffset;
    f -> totalHeight = renderer -> fontSize;

	return f;
}

trueTypeFontRenderer * openTrueTypeRenderer(trueTypeFontDescriptor * fontDescriptor, float fontSize) {
    trueTypeFontRenderer * renderer = fontDescriptor -> rootRenderer;
    while (renderer != NULL) {
        if (renderer -> fontSize == fontSize) {
            renderer -> referenceCounter++;
            return renderer;
        }
        renderer = renderer -> nextRenderer;
    }

    trueTypeFontRenderer * nextRenderer = fontDescriptor -> rootRenderer;
    renderer = new trueTypeFontRenderer;
    renderer -> nextRenderer = nextRenderer;
    renderer -> previousRenderer = NULL;
    fontDescriptor -> rootRenderer = renderer;
    if (nextRenderer != NULL) {
        nextRenderer -> previousRenderer = renderer;
    }

    renderer -> descriptor = fontDescriptor;
    renderer -> fontSize = fontSize;
    if (fontSize <= 30) {renderer -> aliasScale = 6;}
    else if (fontSize <= 120) {renderer -> aliasScale = 3;}
    else {renderer -> aliasScale = 1;}

    int i; for (i = 0; i < 128; i++) {renderer -> cache[i] = NULL;}
    renderer -> referenceCounter = 1;

    fontCharacterCache * character = drawTrueTypeCharacter(' ', renderer);
    renderer -> tabWidth = 4 * character -> advanceWidth;
    renderer -> scale = (fontDescriptor -> glyphInfoCache[' '] -> h) / fontSize;
    return renderer;
}
trueTypeFontDescriptor * openTrueTypeFont(trueTypeFontManager * fontManager, char * fontFileName) {
    trueTypeFontDescriptor * descriptor = fontManager -> descriptor;
    while (descriptor != NULL) {
        if (strcmp(fontFileName, descriptor -> fontFileName) == 0) {
            break;
        }
        descriptor = descriptor -> nextDescriptor;
    }
    if (descriptor == NULL) {
        fileData * fontFileRaw = openFileToBuffer(fontFileName);
        if (fontFileRaw == NULL) {
            return NULL;
        }

        trueTypeFontDescriptor * nextDescriptor = fontManager -> descriptor;
        descriptor = new trueTypeFontDescriptor;
        descriptor -> nextDescriptor = nextDescriptor;
        descriptor -> previousDescriptor = NULL;
        descriptor -> rootRenderer = NULL;
        descriptor -> fontFileName = fontFileName;
        descriptor -> fontFileRaw = fontFileRaw;
        descriptor -> referenceCounter = 0;
        descriptor -> kernTablePointer = (u8*)getTtfTable((ttfRoot *)descriptor -> fontFileRaw, "kern");
        fontManager -> descriptor = descriptor;

        int i; for (i = 0; i < 128; i++) {descriptor -> glyphInfoCache[i] = NULL;}
    }

    descriptor -> referenceCounter++;
    return descriptor;
}

void deleteTrueTypeRenderer(trueTypeFontDescriptor * descriptor, trueTypeFontRenderer * renderer) {
    int i;
    for (i = 0; i < 128; i++) {
        if (renderer -> cache[i] != NULL) {
            deleteFontCharacterCache(renderer -> cache[i]);
        }
    }

    trueTypeFontRenderer * nextRenderer = renderer -> nextRenderer;
    trueTypeFontRenderer * previousRenderer = renderer -> previousRenderer;
    if (nextRenderer != NULL) {
        nextRenderer -> previousRenderer = previousRenderer;
    }
    if (previousRenderer != NULL) {
        previousRenderer -> nextRenderer = nextRenderer;
    }
    else {
        descriptor -> rootRenderer = nextRenderer;
    }

    delete renderer;
}
void deleteTrueTypeDescriptor(trueTypeFontManager * manager, trueTypeFontDescriptor * descriptor) {
    int i;
    for (i = 0; i < 128; i++) {
        if (descriptor -> glyphInfoCache[i] != NULL) {
            deleteGlyphInfo(descriptor -> glyphInfoCache[i]);
        }
    }
    delete descriptor -> fontFileRaw -> buffer;
    delete descriptor -> fontFileRaw;

    trueTypeFontRenderer * renderer = descriptor -> rootRenderer;
    while (renderer != NULL) {
        deleteTrueTypeRenderer(descriptor, renderer);
    }

    trueTypeFontDescriptor * nextDescriptor = descriptor -> nextDescriptor;
    trueTypeFontDescriptor * previousDescriptor = descriptor -> previousDescriptor;
    if (nextDescriptor != NULL) {
        nextDescriptor -> previousDescriptor = previousDescriptor;
    }
    if (previousDescriptor != NULL) {
        previousDescriptor -> nextDescriptor = nextDescriptor;
    }
    else {
        manager -> descriptor = nextDescriptor;
    }

}

void closeTrueTypeDescriptor(trueTypeFontManager * manager, trueTypeFontDescriptor * descriptor) {
    descriptor -> referenceCounter--;
    if (descriptor -> referenceCounter == 0) {
        deleteTrueTypeDescriptor(manager, descriptor);
    }
}
void closeTrueTypeRenderer(trueTypeFontDescriptor * descriptor, trueTypeFontRenderer * renderer) {
    renderer -> referenceCounter--;
    if (renderer -> referenceCounter == 0) {
        deleteTrueTypeRenderer(descriptor, renderer);
    }
}

float getKerningPair(trueTypeFontRenderer * renderer, i16 l, i16 r) {
    u8 * p = renderer -> descriptor -> kernTablePointer;

    if (p == NULL) {
        return 0;
    }
    u16 subtableCount = u2beToLE( *(u16*)(p + 2) );

    int i;
    p += 4;
    for ( i = 0; i < subtableCount; i++) {
        u16 format = u2beToLE(*(u16*)p);
        u16 length = u2beToLE(*(u16*)(p + 2));

        if (format == 0) {
            u16 pairCount = u2beToLE( *(u16*)(p + 6));
            ttfKernSubtableFormat0KerningPair * kp = (ttfKernSubtableFormat0KerningPair *)(p + 14);
            int j;
            for (j = 0; j < pairCount; j++) {
                u16 left = u2beToLE( kp -> left );
                u16 right = u2beToLE( kp -> right );
                if (l == left && r == right) {
                    i16 val = s2beToLE( kp -> value );
                    return ((float)val) / renderer -> scale;
                }

                kp++;
            }
        }
        p += (u32)length;
    }

    return 0;
}


void drawTrueTypeText(trueTypeFontRenderer * renderer, char * text, int x, int y, float maxWidth, bool draw) {

    float width = 0;
    int i = 0;
    while (true) {
        int c = text[i];
        if (c > 0 && c < 127) {
            fontCharacterCache * character = drawTrueTypeCharacter(c, renderer);
            float newWidth = width + character -> advanceWidth;
            if (c == 9) {
                newWidth = ((float)((int)((width + 1.0) / renderer -> tabWidth) + 1)) * renderer -> tabWidth;
            }

            if (newWidth >= maxWidth) {
                break;
            }
            if (draw && c != 32) {
                copyBufferRGBShade(character -> screen, onScreen, 0,0, x + width + character -> xBufferOffset, y + round(character -> yBufferOffset), character -> screen -> windowWidth, character -> screen -> windowHeight);
            }
            width = newWidth;
        }
        else if (c == '\0') {
            return;
        }

        i++;
    }
}