
#include "GraphicsLibrary/Extensions/FileTypes/Jpeg.h"
#include "GraphicsLibrary/Extensions/DataOperations.h"

int zigZagIndexes[] = {	0,	1,	8, 	16,	9,	2,	3,	10,
			17,	24,	32,	25,	18,	11,	4,	5,
			12,	19,	26,	33,	40,	48,	41,	34,
			27,	20,	13,	6,	7,	14,	21,	28,
			35,	42,	49,	56,	57,	50,	43,	36,
			29,	22,	15,	23,	30,	37,	44,	51,
			58,	59,	52,	45,	38,	31,	39,	46,
			53,	60,	61,	54,	47,	55,	62,	63	};

int DCNegativeBases[12] = { 0, -1, -3, -7, -15, -31, -63, -127, -255, -511, -1023, -2047 };

unsigned char JPEGMarker[] = { 0xFF, 0xDA };
unsigned char JPEGFalseMarker[] = { 0xFF, 0x00 };
unsigned char JFIFMarker[] = "JFIF";

void copyQuantizationTable( int * destination, unsigned char * source ) {
	int i;
	for ( i = 0; i < 64; i++ ) {
		destination[i] = source[i];
	}
}

int getTableValuesDC( unsigned char * pointer, int * position, huffmanTable * table ) {

	int bit = position[ 0 ];
	int index = position[ 1 ];
	int i, j, DCBitValue, testValue;

	int value = ( ( (pointer[index] << 8) + pointer[index + 1] ) >> bit) & 0x1FE;
	int * table1 = table -> table1;
	int * table2 = table -> table2;

	DCBitValue = table1[ value ];
	if ( DCBitValue >= 0 ) {
		bit -= table1[ value + 1 ]; if ( bit < 0 ) { index++; bit += 8; }
	}
	else {
		index++;
		int * t2table = table2 + table1[ value + 1 ] * 512;
		int value2 = ( ( (pointer[index] << 8) + pointer[index + 1] ) >> bit) & 0x1FE;
		DCBitValue = t2table[ value2 ];
		bit -= t2table[ value2 + 1 ]; if ( bit < 0 ) { index++; bit += 8; }
	}

	if ( DCBitValue == 0 ) { 
		position[ 0 ] = bit;
		position[ 1 ] = index;
		return 0;
	}
	else {
		bool DCSign = ( pointer[ index ] >> bit ) & 1;

		bit -= DCBitValue;
		value = ( ( (pointer[index] << 16) + (pointer[index + 1] << 8) + pointer[index + 2] ) >> ( 17 + bit ) ) & andValues[ DCBitValue ];
		if ( bit < 0 ) { index += (7 - bit) / 8; bit &= 7; }

		position[ 0 ] = bit;
		position[ 1 ] = index;

		if (!DCSign) { value += DCNegativeBases[ DCBitValue ]; }
		return value;
	}
}

void getTableValuesAC( unsigned char * pointer, int * position, huffmanTable * table, int * qtTable, int * storeData ) {
	int bit = position[ 0 ];
	int index = position[ 1 ];

	int i, j, ACBitValue, value, codeLength;
	int storeIndex = 1;

	int * table1 = table -> table1;
	int * table2 = table -> table2;

	bool ACSign;
	while ( true ) {

		value = ( ( (pointer[index] << 8) + pointer[index + 1] ) >> bit) & 0x1FE;

		ACBitValue = table1[ value ];
		if ( ACBitValue >= 0 ) {
			codeLength = table1[ value + 1 ];
			bit -= codeLength; if ( bit < 0 ) { index++; bit += 8; }

			if ( ACBitValue == 0 ) {
				position[ 0 ] = bit;
				position[ 1 ] = index;
				return;
			}

			storeIndex += ACBitValue >> 4;
			ACBitValue &= 0x0F;
		}
		else {
			index++;
			int * t2table = table2 + table1[ value + 1 ] * 512;
			int value2 = ( ( (pointer[index] << 8) + pointer[index + 1] ) >> bit) & 0x1FE;

			ACBitValue = t2table[ value2 ];
			codeLength = t2table[ value2 + 1 ];
			bit -= codeLength; if ( bit < 0 ) { index++; bit += 8; }

			if ( ACBitValue == 0 ) {
				position[ 0 ] = bit;
				position[ 1 ] = index;
				return;
			}

			storeIndex += ACBitValue >> 4;
			ACBitValue &= 0x0F;
		}

		ACSign = ( pointer[ index ] >> bit ) & 1;

		bit -= ACBitValue;
		value = ( ( (pointer[index] << 8) + pointer[index + 1] ) >> ( 9 + bit ) ) & andValues[ ACBitValue ];
		if ( bit < 0 ) { index += (7 - bit) / 8; bit &= 7; }

		if (!ACSign) { value += DCNegativeBases[ ACBitValue ]; }
		storeData[ zigZagIndexes[ storeIndex ] ] = value * qtTable[ storeIndex ];

		storeIndex++;
		if ( storeIndex > 63 ) {
			position[ 0 ] = bit;
			position[ 1 ] = index;
			return;
		}
	}
}

#define xadd3(xa,xb,xc,xd,h) p=xa+xb, n=xa-xb, xa=p+xc+h, xb=n+xd+h, xc=p-xc+h, xd=n-xd+h // triple-butterfly-add (and possible rounding)
#define xmul(xa,xb,k1,k2,sh) n=k1*(xa+xb), p=xa, xa=(n+(k2-k1)*xb)>>sh, xb=(n-(k2+k1)*p)>>sh // butterfly-mul equ.(2) 

static void idct1(int *x, int *y, int ps, int half) // 1D-IDCT
{
	int p, n;
	x[0]<<=9, x[1]<<=7, x[3]*=181, x[4]<<=9, x[5]*=181, x[7]<<=7;
	xmul(x[6],x[2],277,669,0);
	xadd3(x[0],x[4],x[6],x[2],half);
	xadd3(x[1],x[7],x[3],x[5],0);
	xmul(x[5],x[3],251,50,6);
	xmul(x[1],x[7],213,142,6);
	y[0*8]=(x[0]+x[1])>>ps;
	y[1*8]=(x[4]+x[5])>>ps;
	y[2*8]=(x[2]+x[3])>>ps;
	y[3*8]=(x[6]+x[7])>>ps;
	y[4*8]=(x[6]-x[7])>>ps;
	y[5*8]=(x[2]-x[3])>>ps;
	y[6*8]=(x[4]-x[5])>>ps;
	y[7*8]=(x[0]-x[1])>>ps;
}

void idct(int *b)
{
	int i, b2[64];
	for (i=0; i<8; i++) idct1(b+i*8, b2+i, 9, 1<<8); // row
	for (i=0; i<8; i++) idct1(b2+i*8, b+i, 12, 1<<11); // col 
}

void idct128(int *b)
{
	int i, b2[64];
	for (i=0; i<8; i++) idct1(b+i*8, b2+i, 9, 1<<8); // row
	for (i=0; i<8; i++) idct1(b2+i*8, b+i, 12, 1<<11); // col 
	for ( i = 0; i < 64; i++ ) { b[i] += 128; }
}

uint32_t YCbCrToRGB( int Y, int Cb, int Cr ) {
	int R = Y + ((359 * Cr ) >> 8);							//Y + 1.402 * Cr;
	if ( R > 255 ) { R = 255; } if ( R < 0 ) { R = 0; }
	int G = Y - (( Cb * 88 ) >> 8) - (( Cr * 182 ) >> 8);				//Y - 0.34414 * Cb - 0.71414 * Cr;
	if ( G > 255 ) { G = 255; } if ( G < 0 ) { G = 0; }
	int B = Y + ( (454 * Cb) >> 8 );						//Y + 1.772 * Cb;
	if ( B > 255 ) { B = 255; } if ( B < 0 ) { B = 0; }
	return ( (R) << 16 ) + ( (G) << 8 ) + (B);
}

void print444Macroblocks( unsigned char * data, int ** quantPointers, huffmanTable ** huffPointers, screenData * imageData, int * position, int width, int height, int blocks ) {

	int * YQuant = quantPointers[0];
	int * CbQuant = quantPointers[1];
	int * CrQuant = quantPointers[2];

	huffmanTable * YDC = huffPointers[0];
	huffmanTable * CbDC = huffPointers[1];
	huffmanTable * CrDC = huffPointers[2];
	huffmanTable * YAC = huffPointers[3];
	huffmanTable * CbAC = huffPointers[4];
	huffmanTable * CrAC = huffPointers[5];

	int Y_DC = 0; int Cb_DC = 0; int Cr_DC = 0;
	int YData[64];
	int CbData[64];
	int CrData[64];

	uint32_t * pixels = imageData -> screen;

	int block;
	int blockStart = position[2];
	int blockEnd = blockStart + blocks;
	int blockWidth = width / 8; if ( width % 8 != 0 ) { blockWidth++; }

	int widthMinus8 = width - 8;
	
	int i = (blockStart % blockWidth) * 8;
	int j = (blockStart / blockWidth) * 8;
	for ( block = 0; block < blocks; block++ ) {

		memset( YData, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		YData[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, YData );
		idct128( YData );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC( data, position, CbAC, CrQuant, CbData );
		idct( CbData );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC( data, position, CrAC, CrQuant, CrData );
		idct( CrData );

		int x, y;
		int pixelIndex = width * j + i;
		int k = 0;
		for ( y = 0; y < 8; y++ ) {
			for ( x = 0; x < 8; x++ ) {
				pixels[ pixelIndex ] = YCbCrToRGB( YData[k], CbData[k], CrData[k] );
				pixelIndex++;
				k++;
			}
			pixelIndex += widthMinus8;
		}

		i += 8;
		if ( i >= width ) { j += 8; i = 0; }
	}

	position[2] = blockEnd;
}

void print420Macroblocks( unsigned char * data, int ** quantPointers, huffmanTable ** huffPointers, screenData * imageData, int * position, int width, int height, int blocks ) {

	int * YQuant = quantPointers[0];
	int * CbQuant = quantPointers[1];
	int * CrQuant = quantPointers[2];

	huffmanTable * YDC = huffPointers[0];
	huffmanTable * CbDC = huffPointers[1];
	huffmanTable * CrDC = huffPointers[2];
	huffmanTable * YAC = huffPointers[3];
	huffmanTable * CbAC = huffPointers[4];
	huffmanTable * CrAC = huffPointers[5];

	int Y_DC = 0; int Cb_DC = 0; int Cr_DC = 0;
	int Y00Data[64];	int Y00Final[64];
	int Y10Data[64];	int Y10Final[64];
	int Y01Data[64];	int Y01Final[64];
	int Y11Data[64];	int Y11Final[64];
	int CbData[64];		int CbMid[64];		int CbFinal[256];
	int CrData[64];		int CrMid[64];		int CrFinal[256];

	uint32_t * pixels = imageData -> screen;

	int block;
	int blockStart = position[2];
	int blockEnd = blockStart + blocks;
	int blockWidth = width / 16; if ( width % 16 != 0 ) { blockWidth++; }

	int widthMinus8 = width - 8;
	int widthTimes8 = width * 8;
	
	int i = (blockStart % blockWidth) * 16;
	int j = (blockStart / blockWidth) * 16;
	for ( block = 0; block < blocks; block++ ) {

		memset( Y00Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y00Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y00Data );
		idct128( Y00Data );

		memset( Y10Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y10Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y10Data );
		idct128( Y10Data );

		memset( Y01Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y01Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y01Data );
		idct128( Y01Data );

		memset( Y11Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y11Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y11Data );
		idct128( Y11Data );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC( data, position, CbAC, CrQuant, CbData );
		idct( CbData );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC( data, position, CrAC, CrQuant, CrData );
		idct( CrData );

		int x, y;
		int k = 0;
		int l;
		int pixelIndex = width * j + i;
		for ( y = 0; y < 8; y++ ) {
			for ( x = 0; x < 8; x++ ) {
				l = 8 * (y >> 1) + x / 2;
				pixels[ pixelIndex ] 			= YCbCrToRGB( Y00Data[k], CbData[l], 		CrData[l] );
				pixels[ pixelIndex + 8]			= YCbCrToRGB( Y10Data[k], CbData[l + 4], 	CrData[l + 4] );
				pixels[ pixelIndex + widthTimes8] 	= YCbCrToRGB( Y01Data[k], CbData[l + 32],	CrData[l + 32] );
				pixels[ pixelIndex + widthTimes8 + 8 ]	= YCbCrToRGB( Y11Data[k], CbData[l + 36],	CrData[l + 36] );
				pixelIndex++;
				k++;
			}
			pixelIndex += widthMinus8;
		}

		i += 16;
		if ( i >= width ) { j += 16; i = 0; }
	}

	position[2] = blockEnd;
}

int getWordBE( unsigned char * data ) {
	return ((int)(* data )) * 256 + ((int)(*( data + 1)));
}

screenData * renderJPEGImage( unsigned char * data, int fileSize ) {
	bool reachedEnd = false;

	int dataPrecision;
	int height;
	int width;
	int numberOfComponents;

	int YSample, CbSample, CrSample, samplingFactor;
	int * YQuant;
	int * CbQuant;
	int * CrQuant;
	huffmanTable * YAC; 	huffmanTable * YDC;
	huffmanTable * CbAC;	huffmanTable * CbDC;
	huffmanTable * CrAC;	huffmanTable * CrDC;

	int numberOfQT;
	int SOSindex;

	int qTables[4][64];
	huffmanTable huffmanTablesDC[4];
	huffmanTable huffmanTablesAC[4];
	int i;
	for ( i = 0; i < 4; i++ ) {
		(huffmanTablesAC[i].table1) = NULL;
		(huffmanTablesAC[i].table2) = NULL;
		(huffmanTablesDC[i].table1) = NULL;
		(huffmanTablesDC[i].table2) = NULL;
	}

	screenData * imageData = NULL;

	int renderWidth;
	int renderHeight;
	int position[] = { 7, 0, 0 };
	int restartInterval = 0;

	int index = 0;
	while ( !reachedEnd ) {
		index += 2;
		index += parseData( JPEGMarker, data + index, 1, fileSize );

		if ( *( data + index + 1) == 192 || ( *( data + index + 1) == 0xC2 ) ) {
			int SOFType = ((int)(data[index + 1])) & 0x0F;
			
			if ( SOFType != 0 ) {
				for ( i = 0; i < 4; i++ ) {
					deleteHuffTable( &huffmanTablesAC[i] );
					deleteHuffTable( &huffmanTablesDC[i] );
				}
				return NULL;
			}

			int length = getWordBE( data + index + 2);
			dataPrecision = *( data + index + 4);
			height = getWordBE( data + index + 5);
			width = getWordBE( data + index + 7);
			numberOfComponents = *( data + index + 9 );

			for ( i = 0; i < numberOfComponents; i++ ) {
				int compID = (int)( data[ index + 10 + 3 * i ] );
				int sample = (int)( data[ index + 10 + 3 * i + 1] );
				int quant = (int)( data[ index + 10 + 3 * i + 2] );
				if ( compID == 1 ) { YSample = sample; 	YQuant = qTables[ quant ]; }
				if ( compID == 2 ) { CbSample = sample;	CbQuant = qTables[ quant ]; }
				if ( compID == 3 ) { CrSample = sample; CrQuant = qTables[ quant ]; }
			}

			renderWidth = width;
			renderHeight = height;

			if ( YSample == 0x11 ) {
				if ( CbSample != 0x11 || CrSample != 0x11 ) {
					for ( i = 0; i < 4; i++ ) {
						deleteHuffTable( &huffmanTablesAC[i] );
						deleteHuffTable( &huffmanTablesDC[i] );
					}
					return NULL;
				}
				samplingFactor = 444;
				renderWidth = width;	if ( width % 8 > 0 ) { renderWidth += 8 - width % 8; }
				renderHeight = height;	if ( height % 8 > 0 ) { renderHeight += 8 - height % 8; }
				if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 64; }
			}
			if ( YSample == 0x22 ) {
				if ( CbSample == 0x21 ) {
					if ( CrSample != 0x21 ) {
						for ( i = 0; i < 4; i++ ) {
							deleteHuffTable( &huffmanTablesAC[i] );
							deleteHuffTable( &huffmanTablesDC[i] );
						}
						return NULL;
					}
					samplingFactor = 422;
					renderWidth = width;	if ( width % 16 > 0 ) { renderWidth += 16 - width % 16; }
					renderHeight = height;	if ( height % 8 > 0 ) { renderHeight += 8 - height % 8; }
					if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 128; }
				}
				if ( CbSample == 0x11 ) {
					if ( CrSample != 0x11 ) {
						for ( i = 0; i < 4; i++ ) {
							deleteHuffTable( &huffmanTablesAC[i] );
							deleteHuffTable( &huffmanTablesDC[i] );
						}
						return NULL;
					}
					samplingFactor = 420;
					renderWidth = width;	if ( width % 16 > 0 ) { renderWidth += 16 - width % 16; }
					renderHeight = height;	if ( height % 16 > 0 ) { renderHeight += 16 - height % 16; }
					if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 256; }	
				}
			}

			uint32_t * pixels = new uint32_t[ renderWidth * renderHeight ];
			imageData = new screenData;
			imageData -> screen = pixels;
			imageData -> windowWidth = renderWidth;
			imageData -> windowHeight = renderHeight;
		}

		else if ( *( data + index + 1) >= 0xE0 && *( data + index + 1) < 0xF0 ) {
			int length = getWordBE( data + index + 2);
			index += length;
		}


		else if ( *( data + index + 1) == 0xC4 ) {
			int length = getWordBE( data + index + 2);

			int subIndex = 0;
			while ( subIndex < length - 2 ) {
				int htInfo = *(data + index + subIndex + 4);
				int tableClass = ( htInfo & 0xF0 ) >> 4;
				int tableDest = ( htInfo & 0x0F );

				if ( tableClass ) { subIndex += fillHuffmanTable( &(huffmanTablesAC[ tableDest ]), data + index + subIndex + 5, data + index + subIndex + 21 ); }
				else { subIndex += fillHuffmanTable( &(huffmanTablesDC[ tableDest ]), data + index + subIndex + 5, data + index + subIndex + 21 ); }

				subIndex += 17;
			}
		}
		else if ( *( data + index + 1) == 0xDB ) {
			int length = getWordBE( data + index + 2);

			numberOfQT = (length - 2) / 65;

			for ( i = 0; i < numberOfQT; i++ ) {
				int qtInfo = *(data + index + 4 + 65 * i);
				int elementPrecision = (qtInfo & 0xF0) >> 4;
				if ( elementPrecision ) { 				// will only work for 8 bit precision
					if ( imageData != NULL ) {
						delete imageData -> screen;
						delete imageData;
					}
					for ( i = 0; i < 4; i++ ) {
						deleteHuffTable( &huffmanTablesAC[i] );
						deleteHuffTable( &huffmanTablesDC[i] );
					}
					return NULL;
				}
				int tableDestination = qtInfo & 0x0F;

				copyQuantizationTable( qTables[ tableDestination ], data + index + 5 + 65 * i );
			}
			index += length;
		}
		else if ( *( data + index + 1) == 0xDD ) {
			int restInt = getWordBE( data + index + 4);
			if ( restInt != 0 ) { restartInterval = restInt; }
		}
		else if ( *( data + index + 1) == 0xD9 ) {
			reachedEnd = true;
		}

		else if ( *( data + index + 1) == 0xDA ) {
			int length = getWordBE( data + index + 2);
			int components = *( data + index + 4);

			for ( i = 0; i < components; i++ ) {
				int componentID = *(data + index + 5 + 2 * i);
				int huffmanTblID = *(data + index + 6 + 2 * i);
				int acTable = huffmanTblID & 0x0F;
				int dcTable = (huffmanTblID & 0xF0) >> 4;

				if ( componentID == 1 ) {
					YAC = &huffmanTablesAC[ acTable ];
					YDC = &huffmanTablesDC[ dcTable ];
				}
				if ( componentID == 2 ) {
					CbAC = &huffmanTablesAC[ acTable ];
					CbDC = &huffmanTablesDC[ dcTable ];
				}
				if ( componentID == 3 ) {
					CrAC = &huffmanTablesAC[ acTable ];
					CrDC = &huffmanTablesDC[ dcTable ];
				}
			}

			index += length + 2;
			SOSindex = index;

			int copyIndex = SOSindex;
			int restOfFile = fileSize - SOSindex;

			bool keepParsing = true;

			i = parseData( JPEGFalseMarker, data + index, 1, restOfFile );
			if ( data[ index + i + 1 ] != 0 ) { keepParsing = false; }
			copyIndex += i + 1;
			index += i + 2;

			while ( keepParsing ) {
				i = parseData( JPEGFalseMarker, data + index, 1, restOfFile );
				if ( data[ index + i + 1 ] != 0 ) {
					keepParsing = false;
					memcpy( data + copyIndex, data + index, i );
					index += i;
				}
				else {
					memcpy( data + copyIndex, data + index, i + 1);				
					copyIndex += i + 1;
					index += i + 2;
					restOfFile -= i + 2;
				}
			}

			int * quantPointers[3] = { YQuant, CbQuant, CrQuant };
			huffmanTable * huffPointers[6] = { YDC, CbDC, CrDC, YAC, CbAC, CrAC };

			if ( samplingFactor == 444 ) {
				print444Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			else if ( samplingFactor == 420 ) {	
				print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			index -= 2;
		}

		else if ( *( data + index + 1) >= 0xD0 && *( data + index + 1) < 0xD8 ) {

			index += 2;
			SOSindex = index;

			int copyIndex = SOSindex;
			int restOfFile = fileSize - SOSindex;

			bool keepParsing = true;

			i = parseData( JPEGFalseMarker, data + index, 1, restOfFile );
			if ( data[ index + i + 1 ] != 0 ) { keepParsing = false; }
			copyIndex += i + 1;
			index += i + 2;

			while ( keepParsing ) {
				i = parseData( JPEGFalseMarker, data + index, 1, restOfFile );
				if ( data[ index + i + 1 ] != 0 ) {
					keepParsing = false;
					memcpy( data + copyIndex, data + index, i );
					index += i;
				}
				else {
					memcpy( data + copyIndex, data + index, i + 1);				
					copyIndex += i + 1;
					index += i + 2;
					restOfFile -= i + 2;
				}
			}

			int * quantPointers[3] = { YQuant, CbQuant, CrQuant };
			huffmanTable * huffPointers[6] = { YDC, CbDC, CrDC, YAC, CbAC, CrAC };

			if ( samplingFactor == 444 ) {
				position[0] = 7;
				position[1] = 0;
				print444Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			else if ( samplingFactor == 420 ) {
				position[0] = 7;
				position[1] = 0;
				print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			index -= 2;

		}

	}

	for ( i = 0; i < 4; i++ ) {
		deleteHuffTable( &huffmanTablesAC[i] );
		deleteHuffTable( &huffmanTablesDC[i] );
	}

	return imageData;
}

