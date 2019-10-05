int bitReversals[] =
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

int zigZagIndexes[] = {	0,	1,	8, 	16,	9,	2,	3,	10,
			17,	24,	32,	25,	18,	11,	4,	5,
			12,	19,	26,	33,	40,	48,	41,	34,
			27,	20,	13,	6,	7,	14,	21,	28,
			35,	42,	49,	56,	57,	50,	43,	36,
			29,	22,	15,	23,	30,	37,	44,	51,
			58,	59,	52,	45,	38,	31,	39,	46,
			53,	60,	61,	54,	47,	55,	62,	63	};

int oppAndValues[] = { 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF };
int oppAndValues2[] = { 0xFF80, 0xFFC0, 0xFFE0, 0xFFF0, 0xFFF8, 0xFFFC, 0xFFFE, 0xFFFF };

int andValues[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535 };
int DCNegativeBases[12] = { 0, -1, -3, -7, -15, -31, -63, -127, -255, -511, -1023, -2047 };

struct huffmanTable {
	int * table1;
	int * table2;
};

void deleteHuffTable( huffmanTable * table ) {
	int * table1 = table -> table1;
	if ( table != NULL ) {
		delete table1;
	}
	int * table2 = table -> table2;
	if ( table -> table2 != NULL ) {
		delete table2;
	}
}

int fillHuffmanTable( huffmanTable * table, unsigned char * pointer ) {
	unsigned char * codePointer = pointer + 16;
	int i, j;
	int numberOfEntries = 0;
	for ( i = 0; i < 16; i++ ) {
		numberOfEntries += pointer[ i ];
	}

	int * expandedTable1 = new int[ 512 ];

	cout << expandedTable1 << endl;
	for ( i = 0; i < 512; i++ ) {
		expandedTable1[i] = -1;
	}


	int index = 0;
	int value = 0;
	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < pointer[ i ]; j++ ) {
			int newValue = value << (7 - i);
			int k;
			for ( k = 0; k < (1 << (7 - i)); k++ ) {
				expandedTable1[ 2 * (newValue + k) ] = codePointer[ index ];
				expandedTable1[ 2 * (newValue + k) + 1 ] = i + 1;
			}
			index++;
			value++;
		}
		value <<= 1;
	}

	int numOf16Tables = 0;
	for ( i = 0; i < 256; i++ ) {
		if ( expandedTable1[ 2 * i ] == -1 ) {
			expandedTable1[ 2 * i + 1 ] = numOf16Tables;
			numOf16Tables++;
		}
	}

	int * expandedTable2 = new int[numOf16Tables * 512];
	for ( i = 0; i < numOf16Tables * 512; i++ ) {
		expandedTable2[i] = -1;
	}

	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < pointer[ i + 8 ]; j++ ) {
			int newValue = value << (7 - i);			 
			int * t2table = expandedTable2 + expandedTable1[ 2 * ( newValue >> 8 ) + 1] * 512;

			int k;
			for ( k = 0; k < (1 << (7 - i)); k++ ) {
				t2table[ ( (newValue & 0xFF) + k ) * 2 ] = codePointer[ index ];
				t2table[ ( (newValue & 0xFF) + k) * 2 + 1 ] = i + 1;
			}

			index++;
			value++;
		}
		value <<= 1;
	}

	table -> table1 = expandedTable1;
	table -> table2 = expandedTable2;
	
	return numberOfEntries;
}

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
		//cout << index << "\t" << bit << "\t" << (bit & 7) << endl;
		if ( bit < 0 ) { index += (7 - bit) / 8; bit &= 7; }
		//cout << index << "\t" << bit << endl << endl;

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

void decode444Macroblocks( unsigned char * data, int * blockData, int ** quantPointers, huffmanTable ** huffPointers, int * position, int blocks ) {

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

	int block;
	int blockStart = position[2];
	int blockEnd = blockStart + blocks;

	for ( block = blockStart; block < blockEnd; block++ ) {

		int * YData = blockData + block * 192;
		int * CbData = YData + 64;
		int * CrData = YData + 128;

		memset( YData, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		YData[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, YData );
		//idct128( YData );
		//cout << "Y:"; printBlock( YData );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC( data, position, CbAC, CrQuant, CbData );
		//idct( CbData );
		//cout << "Cb:"; printBlock( CbData );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC( data, position, CrAC, CrQuant, CrData );
		//idct( CrData );
		//cout << "Cr:"; printBlock( CrData );
	}

	position[2] = blockEnd;
}

void display444Macroblocks( int * blockData, screenData * imageData, int width, int height, int * position, int blocks ) {

	uint32_t * pixels = imageData -> screen;

	int block;
	int blockStart = position[2];
	int blockEnd = blockStart + blocks;
	int blockWidth = width / 8; if ( width % 8 != 0 ) { blockWidth++; }

	int widthMinus8 = width - 8;
	
	int i = (blockStart % blockWidth) * 8;
	int j = (blockStart / blockWidth) * 8;
	for ( block = blockStart; block < blockEnd; block++ ) {

		int * YData = blockData + block * 192;
		int * CbData = YData + 64;
		int * CrData = YData + 128;

		idct128( YData );
		idct( CbData );
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
	cout << blockStart << endl;
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
		idct128( Y00Data );	//inverseDCTWithTableWithAdd( Y00Data, Y00Final );
		//cout << "Y00:"; printBlock( Y00Data );

		memset( Y10Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y10Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y10Data );
		idct128( Y10Data );	//inverseDCTWithTableWithAdd( Y10Data, Y10Final );
		//cout << "Y10:"; printBlock( Y10Data );

		memset( Y01Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y01Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y01Data );
		idct128( Y01Data );
		//cout << "Y01:"; printBlock( Y01Data );

		memset( Y11Data, 0, 256 );
		Y_DC += getTableValuesDC( data, position, YDC );
		Y11Data[0] = Y_DC * YQuant[0];
		getTableValuesAC( data, position, YAC, YQuant, Y11Data );
		idct128( Y11Data );
		//cout << "Y11:"; printBlock( Y11Data );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC( data, position, CbAC, CrQuant, CbData );
		idct( CbData );
		//cout << "Cb:"; printBlock( CbData );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC( data, position, CrAC, CrQuant, CrData );
		idct( CrData );
		//cout << "Cr:"; printBlock( CrData );

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

	cout << blockEnd << endl;
	position[2] = blockEnd;
}

unsigned char JPEGMarker[] = { 0xFF, 0xDA };
unsigned char JPEGFalseMarker[] = { 0xFF, 0x00 };
unsigned char JFIFMarker[] = "JFIF";

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
	int * blockPointers;

	int renderWidth;
	int renderHeight;
	int position1[] = { 7, 0, 0 };
	int position2[] = { 7, 0, 0 };
	int restartInterval = 0;

	int index = 0;
	while ( !reachedEnd ) {
		index += 2;
		index += parseData( JPEGMarker, data + index, 1, fileSize );

		if ( *( data + index + 1) == 192 || ( *( data + index + 1) == 0xC2 ) ) {
			int SOFType = ((int)(data[index + 1])) & 0x0F;
			cout << "SOF" << SOFType << " @ " << index << endl;
			if ( SOFType != 0 ) {
				cout << "Bad SOF Type" << endl;
				for ( i = 0; i < 4; i++ ) {
					deleteHuffTable( &huffmanTablesAC[i] );
					deleteHuffTable( &huffmanTablesDC[i] );
				}
				return NULL;
			}

			int length = getWordBE( data + index + 2);	cout << "length is:\t" << length << endl;
			dataPrecision = *( data + index + 4); 		cout << "Data precision:\t" << dataPrecision << endl;
			height = getWordBE( data + index + 5);		cout << "Image height:\t" << height << endl;
			width = getWordBE( data + index + 7); 		cout << "Image width:\t" << width << endl;
			numberOfComponents = *( data + index + 9 ); 	cout << "#ofComponents:\t" << numberOfComponents << endl;

			for ( i = 0; i < numberOfComponents; i++ ) {
				int compID = (int)( data[ index + 10 + 3 * i ] ); 	cout << "Component ID:\t" << compID << endl;
				int sample = (int)( data[ index + 10 + 3 * i + 1] ); 	cout << "\tSamp:\t" << sample << endl;
				int quant = (int)( data[ index + 10 + 3 * i + 2] ); 	cout << "\tQuant:\t" << quant << endl;
				if ( compID == 1 ) { YSample = sample; 	YQuant = qTables[ quant ]; }
				if ( compID == 2 ) { CbSample = sample;	CbQuant = qTables[ quant ]; }
				if ( compID == 3 ) { CrSample = sample; CrQuant = qTables[ quant ]; }
			}

			renderWidth = width;
			renderHeight = height;
			int numOfBlocks;

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
				numOfBlocks = renderWidth * renderHeight / 64;
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
					numOfBlocks = renderWidth * renderHeight / 128;
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
					numOfBlocks = renderWidth * renderHeight / 256;
					if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 256; cout << restartInterval << endl; }	
				}
			}

			blockPointers = new int[numOfBlocks * 192];

			uint32_t * pixels = new uint32_t[ renderWidth * renderHeight ];
			imageData = new screenData;
			imageData -> screen = pixels;
			imageData -> windowWidth = renderWidth;
			imageData -> windowHeight = renderHeight;

			cout << endl;
		}

		else if ( *( data + index + 1) >= 0xE0 && *( data + index + 1) < 0xF0 ) {
			cout << "APP marker @ " << index << endl;
			int length = getWordBE( data + index + 2); //cout << "length is:\t" << length << endl;
			index += length;

			cout << endl;
		}


		else if ( *( data + index + 1) == 0xC4 ) {
			cout << "DHT @ " << index << endl;
			int length = getWordBE( data + index + 2); //cout << "length is:\t" << length << endl;

			int subIndex = 0;
			while ( subIndex < length - 2 ) {
				//cout << "DHT Table @ " << index + subIndex + 5 << endl;

				int htInfo = *(data + index + subIndex + 4);	cout << "\tHTinfo:\t" << htInfo << endl;
				int tableClass = ( htInfo & 0xF0 ) >> 4; 	cout << "\ttbCl:\t" << tableClass << endl;
				int tableDest = ( htInfo & 0x0F ); 		cout << "\ttbDst:\t" << tableDest << endl;

				if ( tableClass ) { subIndex += fillHuffmanTable( &(huffmanTablesAC[ tableDest ]), data + index + subIndex + 5 ); }
				else { subIndex += fillHuffmanTable( &(huffmanTablesDC[ tableDest ]), data + index + subIndex + 5 ); }

				subIndex += 17;
			}
			//cout << index + subIndex + 5 << endl;

			cout << endl;
		}
		else if ( *( data + index + 1) == 0xDB ) {
			cout << "DQT @ " << index << endl;
			int length = getWordBE( data + index + 2); //cout << "length is:\t" << length << endl;

			numberOfQT = (length - 2) / 65;

			for ( i = 0; i < numberOfQT; i++ ) {
				int qtInfo = *(data + index + 4 + 65 * i); 		cout << "QT information:\t" << qtInfo << endl;
				int elementPrecision = (qtInfo & 0xF0) >> 4;		cout << "\teP:\t" << elementPrecision << endl;
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
				int tableDestination = qtInfo & 0x0F; 			cout << "\ttD:\t" << tableDestination << endl;

				copyQuantizationTable( qTables[ tableDestination ], data + index + 5 + 65 * i );
			}
			index += length;

			cout << endl;
		}
		else if ( *( data + index + 1) == 0xDD ) {
			cout << "DRI @ " << index << endl;
			int restInt = getWordBE( data + index + 4);  cout << "restartInt:\t" << restartInterval << endl;
			if ( restInt != 0 ) { restartInterval = restInt; }
			cout << endl;
		}
		else if ( *( data + index + 1) == 0xD9 ) {
			cout << "End @ " << index << endl;
			reachedEnd = true;
		}

		else if ( *( data + index + 1) == 0xDA ) {
			cout << "SOS @ " << index << endl;
			int length = getWordBE( data + index + 2); 	//cout << "length is:\t" << length << endl;
			int components = *( data + index + 4); 		cout << "components:\t" << components << endl;

			for ( i = 0; i < components; i++ ) {
				int componentID = *(data + index + 5 + 2 * i); 	cout << " \tCID:\t" << componentID << endl;
				int huffmanTblID = *(data + index + 6 + 2 * i); cout << " \tHTbl:\t" << huffmanTblID << endl;
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
			cout << "SOS @" << SOSindex << endl;

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
				decode444Macroblocks( data + SOSindex, blockPointers, quantPointers, huffPointers, position1, restartInterval );
				//display444Macroblocks( blockPointers, imageData, renderWidth, renderHeight, position2, restartInterval );
			}

			else if ( samplingFactor == 420 ) {	
				//print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			index -= 2;
		}

		else if ( *( data + index + 1) >= 0xD0 && *( data + index + 1) < 0xD8 ) {

			index += 2;
			SOSindex = index;
			cout << "SOS @" << SOSindex << endl;

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
				position1[0] = 7;
				position1[1] = 0;
				position2[0] = 7;
				position2[1] = 0;
				//decode444Macroblocks( data + SOSindex, blockPointers, quantPointers, huffPointers, position1, restartInterval );
				//display444Macroblocks( blockPointers, imageData, renderWidth, renderHeight, position2, restartInterval );
			}

			else if ( samplingFactor == 420 ) {
				position1[0] = 7;
				position1[1] = 0;
				position2[0] = 7;
				position2[1] = 0;
				//print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			index -= 2;

		}

	}

	for ( i = 0; i < 4; i++ ) {
		deleteHuffTable( &huffmanTablesAC[i] );
		deleteHuffTable( &huffmanTablesDC[i] );
	}

	cout << "WORKED" << endl;

	return imageData;
}
