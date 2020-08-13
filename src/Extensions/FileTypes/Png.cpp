
#include "GraphicsLibrary/Extensions/DataOperations.h"
#include "GraphicsLibrary/Extensions/FileTypes/Png.h"

int readIntBigEndian( unsigned char * data ) {
	unsigned char value[4];
	value[0] = data[3];
	value[1] = data[2];
	value[2] = data[1];
	value[3] = data[0];
	return *(int *)(value);
}

unsigned char IHDR[] = "IHDR";
unsigned char PLTE[] = "PLTE";
unsigned char IDAT[] = "IDAT";
unsigned char bKGD[] = "bKGD";

int getNBits( unsigned char * data, int bit, int len ) {
	int value = *(int*)(data);
	value >>= bit;
	value &= andValues[len];
	return value;
}

int getNBitsFlip( unsigned char * data, int bit, int len ) {
	int value = *(int*)(data);
	value >>= bit;
	int i;
	int value2 = 0;
	for ( i = 0; i < len; i++ ) {
		value2 <<= 1;
		value2 += value & 1;
		value >> 1;
	}
	return value2;
}

huffmanTable * makeHuffTable( int * sourceTable, int length ) {
	int i, j, k;
	int indexLengths[16];
	int indexBases[16];
	for ( i = 0; i < 16; i++ ) { 
		indexLengths[i] = 0;
		indexBases[i] = 0;
	}

	for ( i = 0; i < length; i++ ) {			// makes a table of how many codes there are of each length
		if ( sourceTable[ 2 * i ] != 0 ) {		// length of 0 is not included in the table
			indexLengths[ sourceTable[ 2 * i ] - 1 ]++;
		}
	}

	int index = 0;
	for ( i = 0; i < 16; i++ ) {				// determines the start index for codes of each length in the new source table
		indexBases[i] = index;
		index += indexLengths[i];
	}

	int * newSourceTable = new int[ length ];
	for ( i = 0; i < length; i++ ) {
		int indexLen = sourceTable[2 * i];
		if ( indexLen != 0 ) {
			indexLen--;
			int value = sourceTable[ 2 * i + 1 ];
			newSourceTable[ indexBases[ indexLen ] ] = value;
			indexBases[ indexLen ]++;
		}
	}

	huffmanTable * table = new huffmanTable;
	fillHuffTableIntSource( table, indexLengths, newSourceTable );
	delete newSourceTable;

	return table;
}

void makeHuffTable16( int * sourceTable, int length ) {

	int i, j, k;
	int indexLengths[16];
	int indexBases[16];
	for ( i = 0; i < 16; i++ ) { 
		indexLengths[i] = 0;
		indexBases[i] = 0;
	}

	for ( i = 0; i < length; i++ ) {			// makes a table of how many codes there are of each length
		if ( sourceTable[ 2 * i ] != 0 ) {
			indexLengths[ sourceTable[ 2 * i ] - 1 ]++;
		}
	}

	int index = 0;
	for ( i = 0; i < 16; i++ ) {				// determines the start index for codes of each length in the new source table
		indexBases[i] = index;
		index += indexLengths[i];
	}

	int * newSourceTable = new int[ length ];
	for ( i = 0; i < length; i++ ) {
		int indexLen = sourceTable[2 * i];
		if ( indexLen != 0 ) {
			indexLen--;
			int value = sourceTable[ 2 * i + 1 ];
			newSourceTable[ indexBases[ indexLen ] ] = value;
			indexBases[ indexLen ]++;
		}
	}

	huffmanTable * table = new huffmanTable;
	fillHuffTableIntSource( table, indexLengths, newSourceTable );
}

int lenExtraBitTable[29] = { 0, 0, 0, 0, 0, 0, 0, 0,
			1, 1, 1, 1, 2, 2, 2, 2, 
			3, 3, 3, 3, 4, 4, 4, 4, 
			5, 5, 5, 5, 0 };

int lenExtraBitBases[29] = { 3, 4, 5, 6, 7, 8, 9, 10,
			11, 13, 15, 17, 19, 23, 27, 31,
			35, 43, 51, 59, 67, 83, 99, 115,
			131, 163, 195, 227, 258 };

int distExtraBitTable[30] = { 0, 0, 0, 0, 1, 1, 2, 2,
			3, 3, 4, 4, 5, 5, 6, 6,
			7, 7, 8, 8, 9, 9, 10, 10,
			11, 11, 12, 12, 13, 13 };

int distExtraBitBases[30] = { 1, 2, 3, 4, 5, 7, 9, 13,
			17, 25, 33, 49, 65, 97, 129, 193,
			257, 385, 513, 769, 1025, 1537, 2049, 3073,
			4097, 6145, 8193, 12289, 16385, 24577 };

void decodeHuffmanBlock( unsigned char * data, int * position, huffmanTable * literalTable, huffmanTable * distanceTable, unsigned char * outData ) {

	int bit = position[0];
	int index = position[1];
	int outIndex = position[2];

	int * litTable1 = literalTable -> table1;
	int * litTable2 = literalTable -> table2;
	int * distTable1 = distanceTable -> table1;
	int * distTable2 = distanceTable -> table2;

	int data1, data2;
	int * table2pointer;

	int i, value, j;
	while (true) {

		data1 = getNBits( data + index, bit, 8 );
		value = litTable1[ 2 * data1 ];

		if ( value == -1 ) {
			index++;

			table2pointer = litTable2 + litTable1[ 2 * data1 + 1 ] * 512;
			data2 = getNBits( data + index, bit, 8 );
			value = table2pointer[ 2 * data2 ];

			bit += table2pointer[ 2 * data2 + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; } 
		}
		else {
			bit += litTable1[ 2 * data1 + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; } 
		}

		if ( value < 256 ) { // copy to output stream
			outData[ outIndex ] = value & 0xFF;
			outIndex++;
		}
		else if ( value == 256 ) {
			break;	// end of block
		}
		else {				// length distance combination
			value -= 257;

			int extraBitNum = lenExtraBitTable[ value ];
			int length = lenExtraBitBases[ value ] + getNBits( data + index, bit, extraBitNum );

			bit += extraBitNum; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

			data1 = getNBits( data + index, bit, 8 );
			value = distTable1[ 2 * data1 ];

			if ( value == -1 ) {
				index++;
				
				table2pointer = distTable2 + distTable1[ 2 * data1 + 1 ] * 512;
				data2 = getNBits( data + index, bit, 8 );
				value = table2pointer[ 2 * data2 ];

				bit += table2pointer[ 2 * data2 + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; } 
			}
			else {
				bit += distTable1[ 2 * data1 + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; } 
			}

			extraBitNum = distExtraBitTable[ value ];
			int distance = distExtraBitBases[ value ] + getNBits( data + index, bit, extraBitNum );

			bit += extraBitNum; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

			int sourceIndex = outIndex - distance;
			for ( i = 0; i < length; i++ ) {
				outData[ outIndex ] = outData[ sourceIndex ];
				outIndex++;
				sourceIndex++;
			}
		}
	}

	position[0] = bit;
	position[1] = index;
	position[2] = outIndex;
}

int codeLengthCodes[] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };

void decodeNonCompressedBlock( unsigned char * data, int * position, unsigned char * outData ) {
	int bit = position[0];
	int index = position[1];

	if ( bit != 0 ) { index++; bit = 0; }

	int LEN = 256 * data[index + 1] + data[index]; index += 2;
	int NLEN = 256 * data[index + 1] + data[index]; index+= 2;
}

void decodeBlockHuffmanDynamic( unsigned char * data, int * position, unsigned char * outData ) {

	int bit = position[0];
	int index = position[1];

	int HLIT = getNBits( data + index , bit, 5 );	//cout << "HLIT\t" << HLIT << endl;
	bit += 5; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

	int HDIST = getNBits( data + index , bit, 5 );	//cout << "HDIST\t" << HDIST << endl;
	bit += 5; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

	int HCLEN = getNBits( data + index , bit, 4 );	//cout << "HCLEN\t" << HCLEN << endl << endl;
	bit += 4; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

	HCLEN += 4;
	int i, j;
	int codeLengthCodeTable[38]; for ( i = 0; i < 38; i++ ) { codeLengthCodeTable[i] = 0; }
	for ( i = 0; i < HCLEN; i++ ) {
		int codeIndex = codeLengthCodes[i];
		codeLengthCodeTable[ 2 * codeIndex ] = getNBits( data + index, bit, 3 );
		codeLengthCodeTable[ 2 * codeIndex + 1 ] = codeLengthCodes[i];
		bit += 3; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
	}

	int * codeLengthHuffTable = makeHuffTable( codeLengthCodeTable, 19 ) -> table1;

	int * literalTable = new int[ (HLIT + 257) * 2 ];
	int * distanceTable = new int[ (HDIST + 1) * 2 ];

	int value;
	int lastCodeLength = 0;
	for ( i = 0; i < HLIT + 257; i++ ) {
		value = getNBits( data + index, bit, 8 );
		int codeLength = codeLengthHuffTable[ 2 * value ];
		bit += codeLengthHuffTable[ 2 * value + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

		if ( codeLength == 16 ) {
			int repeats = 3 + getNBits( data + index, bit, 2 );
			bit += 2; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				literalTable[ 2 * i ] = lastCodeLength;
				literalTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else if ( codeLength == 17 ) {
			int repeats = 3 + getNBits( data + index, bit, 3 );
			bit += 3; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				literalTable[ 2 * i ] = 0;
				literalTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else if ( codeLength == 18 ) {
			int repeats = 11 + getNBits( data + index, bit, 7 );
			bit += 7; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				literalTable[ 2 * i ] = 0;
				literalTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else {
			literalTable[ 2 * i ] = codeLength;
			literalTable[ 2 * i + 1] = i;
			lastCodeLength = codeLength;
		}
	}

	lastCodeLength = 0;
	for ( i = 0; i < HDIST + 1; i++ ) {
		value = getNBits( data + index, bit, 8 );
		int codeLength = codeLengthHuffTable[ 2 * value ];
		bit += codeLengthHuffTable[ 2 * value + 1 ]; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

		if ( codeLength == 16 ) {
			int repeats = 3 + getNBits( data + index, bit, 2 );
			bit += 2; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				distanceTable[ 2 * i ] = lastCodeLength;
				distanceTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else if ( codeLength == 17 ) {
			int repeats = 3 + getNBits( data + index, bit, 3 );
			bit += 3; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				distanceTable[ 2 * i ] = 0;
				distanceTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else if ( codeLength == 18 ) {
			int repeats = 11 + getNBits( data + index, bit, 7 );
			bit += 7; if ( bit > 7 ) { index += bit / 8; bit &= 7; }
			for ( j = 0; j < repeats; j++ ) {
				distanceTable[ 2 * i ] = 0;
				distanceTable[ 2 * i + 1] = i;
				i++;
			}
			i--;
		}
		else {
			distanceTable[ 2 * i ] = codeLength;
			distanceTable[ 2 * i + 1] = i;
			lastCodeLength = codeLength;
		}
	}

	huffmanTable * literalHuffmanTable = makeHuffTable( literalTable, HLIT + 257 );
	huffmanTable * distanceHuffmanTable = makeHuffTable( distanceTable, HDIST + 1 );
	delete literalTable;
	delete distanceTable;

	position[0] = bit;
	position[1] = index;

	decodeHuffmanBlock( data, position, literalHuffmanTable, distanceHuffmanTable, outData );

	bit = position[0];
	index = position[1];
}

void displayPNGColorType2( int width, int height, unsigned char * outData, uint32_t * pixels ) {

	int i, j;
	int byteWidth = 3 * width + 1;
	int rowIndex = 0;
	for ( j = 0; j < height; j++ ) {
		int filterType = outData[ rowIndex ];

		if ( filterType == 1 ) {
			for ( i = 1; i < width; i++ ) {
				outData[ rowIndex + 3 * i + 1] += outData[ rowIndex + 3 * i - 2];
				outData[ rowIndex + 3 * i + 2] += outData[ rowIndex + 3 * i - 1];
				outData[ rowIndex + 3 * i + 3] += outData[ rowIndex + 3 * i ];
			}
		}
		else if ( filterType == 2 ) {
			for ( i = 0; i < width; i++ ) {
				outData[ rowIndex + 3 * i + 1] += outData[ rowIndex + 3 * i + 1 - byteWidth ];
				outData[ rowIndex + 3 * i + 2] += outData[ rowIndex + 3 * i + 2 - byteWidth ];
				outData[ rowIndex + 3 * i + 3] += outData[ rowIndex + 3 * i + 3 - byteWidth ];
			}
		}
		else if ( filterType == 3 ) {
			outData[ rowIndex + 1] += outData[ rowIndex + 1 - byteWidth ] / 2;
			outData[ rowIndex + 2] += outData[ rowIndex + 2 - byteWidth ] / 2;
			outData[ rowIndex + 3] += outData[ rowIndex + 3 - byteWidth ] / 2;

			for ( i = 1; i < width; i++ ) {
				outData[ rowIndex + 3 * i + 1] += ( ( outData[ rowIndex + 3 * i - 2] + outData[ rowIndex + 3 * i + 1 - byteWidth ] ) / 2 ) & 0xFF;
				outData[ rowIndex + 3 * i + 2] += ( ( outData[ rowIndex + 3 * i - 1] + outData[ rowIndex + 3 * i + 2 - byteWidth ] ) / 2 ) & 0xFF;
				outData[ rowIndex + 3 * i + 3] += ( ( outData[ rowIndex + 3 * i    ] + outData[ rowIndex + 3 * i + 3 - byteWidth ] ) / 2 ) & 0xFF;
			}
		}

		else if ( filterType == 4 ) {

			outData[ rowIndex + 1 ] += outData[ rowIndex + 1 - byteWidth ];
			outData[ rowIndex + 2 ] += outData[ rowIndex + 2 - byteWidth ];
			outData[ rowIndex + 3 ] += outData[ rowIndex + 3 - byteWidth ];

			int A, B, C, p, dA, dB, dC;
			for ( i = 1; i < width; i++ ) {
				A = outData[ rowIndex + 3 * i - 2];
				B = outData[ rowIndex + 3 * i + 1 - byteWidth];
				C = outData[ rowIndex + 3 * i - 2 - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 3 * i + 1] = ( outData[ rowIndex + 3 * i + 1] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 3 * i + 1] = ( outData[ rowIndex + 3 * i + 1] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 3 * i + 1] = ( outData[ rowIndex + 3 * i + 1] + C ) & 0xFF; }

				A = outData[ rowIndex + 3 * i - 1];
				B = outData[ rowIndex + 3 * i + 2 - byteWidth];
				C = outData[ rowIndex + 3 * i - 1 - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 3 * i + 2] = ( outData[ rowIndex + 3 * i + 2] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 3 * i + 2] = ( outData[ rowIndex + 3 * i + 2] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 3 * i + 2] = ( outData[ rowIndex + 3 * i + 2] + C ) & 0xFF; }

				A = outData[ rowIndex + 3 * i];
				B = outData[ rowIndex + 3 * i + 3 - byteWidth];
				C = outData[ rowIndex + 3 * i - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 3 * i + 3] = ( outData[ rowIndex + 3 * i + 3] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 3 * i + 3] = ( outData[ rowIndex + 3 * i + 3] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 3 * i + 3] = ( outData[ rowIndex + 3 * i + 3] + C ) & 0xFF; }
			}
		}

		int r,g,b;
		for ( i = 0; i < width; i++ ) {
				r = outData[ rowIndex + 3 * i + 1];
				g = outData[ rowIndex + 3 * i + 2];
				b = outData[ rowIndex + 3 * i + 3];
				pixels[ j * width + i ] = ( r << 16 ) + ( g << 8 ) + b; 
		}
		rowIndex += byteWidth;
	}
}

void displayPNGColorType6( int width, int height, unsigned char * outData, uint32_t * pixels ) {

	int i, j;
	int byteWidth = 4 * width + 1;
	int rowIndex = 0;
	for ( j = 0; j < height; j++ ) {
		int filterType = outData[ rowIndex ];

		if ( filterType == 1 ) {
			for ( i = 1; i < width; i++ ) {
				outData[ rowIndex + 4 * i + 1] += outData[ rowIndex + 4 * i - 3];
				outData[ rowIndex + 4 * i + 2] += outData[ rowIndex + 4 * i - 2];
				outData[ rowIndex + 4 * i + 3] += outData[ rowIndex + 4 * i - 1];
				outData[ rowIndex + 4 * i + 4] += outData[ rowIndex + 4 * i ];
			}
		}
		else if ( filterType == 2 ) {
			for ( i = 0; i < width; i++ ) {
				outData[ rowIndex + 4 * i + 1] += outData[ rowIndex + 4 * i + 1 - byteWidth ];
				outData[ rowIndex + 4 * i + 2] += outData[ rowIndex + 4 * i + 2 - byteWidth ];
				outData[ rowIndex + 4 * i + 3] += outData[ rowIndex + 4 * i + 3 - byteWidth ];
				outData[ rowIndex + 4 * i + 4] += outData[ rowIndex + 4 * i + 4 - byteWidth ];
			}
		}

		else if ( filterType == 3 ) {
			outData[ rowIndex + 1] += outData[ rowIndex + 1 - byteWidth ] / 2;
			outData[ rowIndex + 2] += outData[ rowIndex + 2 - byteWidth ] / 2;
			outData[ rowIndex + 3] += outData[ rowIndex + 3 - byteWidth ] / 2;
			outData[ rowIndex + 4] += outData[ rowIndex + 4 - byteWidth ] / 2;

			for ( i = 1; i < width; i++ ) {
				outData[ rowIndex + 4 * i + 1] += ( ( outData[ rowIndex + 4 * i - 3] + outData[ rowIndex + 4 * i + 1 - byteWidth ] ) / 2 ) & 0xFF;
				outData[ rowIndex + 4 * i + 2] += ( ( outData[ rowIndex + 4 * i - 2] + outData[ rowIndex + 4 * i + 2 - byteWidth ] ) / 2 ) & 0xFF;
				outData[ rowIndex + 4 * i + 3] += ( ( outData[ rowIndex + 4 * i - 1] + outData[ rowIndex + 4 * i + 3 - byteWidth ] ) / 2 ) & 0xFF;
				outData[ rowIndex + 4 * i + 4] += ( ( outData[ rowIndex + 4 * i    ] + outData[ rowIndex + 4 * i + 4 - byteWidth ] ) / 2 ) & 0xFF;
			}
		}

		else if ( filterType == 4 ) {

			outData[ rowIndex + 1 ] += outData[ rowIndex + 1 - byteWidth ];
			outData[ rowIndex + 2 ] += outData[ rowIndex + 2 - byteWidth ];
			outData[ rowIndex + 3 ] += outData[ rowIndex + 3 - byteWidth ];
			outData[ rowIndex + 4 ] += outData[ rowIndex + 4 - byteWidth ];

			int A, B, C, p, dA, dB, dC;
			for ( i = 1; i < width; i++ ) {
				A = outData[ rowIndex + 4 * i - 3];
				B = outData[ rowIndex + 4 * i + 1 - byteWidth];
				C = outData[ rowIndex + 4 * i - 3 - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 4 * i + 1] = ( outData[ rowIndex + 4 * i + 1] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 4 * i + 1] = ( outData[ rowIndex + 4 * i + 1] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 4 * i + 1] = ( outData[ rowIndex + 4 * i + 1] + C ) & 0xFF; }

				A = outData[ rowIndex + 4 * i - 2];
				B = outData[ rowIndex + 4 * i + 2 - byteWidth];
				C = outData[ rowIndex + 4 * i - 2 - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 4 * i + 2] = ( outData[ rowIndex + 4 * i + 2] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 4 * i + 2] = ( outData[ rowIndex + 4 * i + 2] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 4 * i + 2] = ( outData[ rowIndex + 4 * i + 2] + C ) & 0xFF; }

				A = outData[ rowIndex + 4 * i - 1 ];
				B = outData[ rowIndex + 4 * i + 3 - byteWidth];
				C = outData[ rowIndex + 4 * i - 1 - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 4 * i + 3] = ( outData[ rowIndex + 4 * i + 3] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 4 * i + 3] = ( outData[ rowIndex + 4 * i + 3] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 4 * i + 3] = ( outData[ rowIndex + 4 * i + 3] + C ) & 0xFF; }

				A = outData[ rowIndex + 4 * i ];
				B = outData[ rowIndex + 4 * i + 4 - byteWidth];
				C = outData[ rowIndex + 4 * i - byteWidth];
				p = A + B - C;
				dA = abs(p - A); dB = abs(p - B); dC = abs(p - C);
				if ( dA <= dB && dA <= dC ) { 	outData[ rowIndex + 4 * i + 4] = ( outData[ rowIndex + 4 * i + 4] + A ) & 0xFF; }
				else if ( dB <= dC ) { 		outData[ rowIndex + 4 * i + 4] = ( outData[ rowIndex + 4 * i + 4] + B ) & 0xFF; }
				else { 				outData[ rowIndex + 4 * i + 4] = ( outData[ rowIndex + 4 * i + 4] + C ) & 0xFF; }
			}
		}

		int a,r,g,b;
		for ( i = 0; i < width; i++ ) {
				r = outData[ rowIndex + 4 * i + 1];
				g = outData[ rowIndex + 4 * i + 2];
				b = outData[ rowIndex + 4 * i + 3];
				a = outData[ rowIndex + 4 * i + 4];
				pixels[ j * width + i ] = (a << 24) + ( r << 16 ) + ( g << 8 ) + b;
		};
		rowIndex += byteWidth;
	}
}

screenData * renderPNGImage( unsigned char * data, int fileSize ) {
	//data += 8;
	int index = 8;

	int width, height;
	int bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;

	int IDATIndex = 0;
	int IDATPointers[1024][2];

	while ( index < fileSize ) {
		int length = readIntBigEndian( data + index );
		index += 8;

		if ( compareData( data + index - 4, IHDR, 4 ) ) {
			width = readIntBigEndian( data + index );
			height = readIntBigEndian( data + index + 4 );

			bitDepth = data[ index + 8 ];
			colorType = data[ index + 9 ];
			compressionMethod = data[ index + 10 ];
			filterMethod = data[ index + 11 ];
			interlaceMethod = data[ index + 12 ];
		}
		else if ( compareData( data + index - 4, PLTE, 4 ) ) {
			// TODO -- implement
		}
		else if ( compareData( data + index - 4, IDAT, 4 ) ) {
			IDATPointers[ IDATIndex ][0] = index;
			IDATPointers[ IDATIndex ][1] = length;
			IDATIndex++;
		}
		else if ( compareData( data + index - 4, bKGD, 4 ) ) {
			// TODO -- implement
		}

		index += length + 4;
	}

	int nextIndex = 0;
	int i, j;
	for ( i = 0; i < IDATIndex; i++ ) {
		memcpy( data + nextIndex, data + IDATPointers[i][0], IDATPointers[i][1] );
		nextIndex += IDATPointers[i][1];
	}

	unsigned char * outData;
	if ( colorType == 2 ) {
		outData = new unsigned char[ ( 3 * width + 1 ) * height ];
	}
	else if ( colorType == 6 ) {
		outData = new unsigned char[ ( 4 * width + 1 ) * height ];
	}

	uint32_t * pixels = new uint32_t[ ( width ) * height ];

	index = 0;
	int bit = 0;
	int outIndex = 0;

	int CMF = getNBits( data + index, bit, 8 ); index++;
	int CM = CMF & 0x0F;
	int CINFO = CMF >> 4;

	int FLG = getNBits( data + index, bit, 8 ); index++;;
	if ( ( (256 * CMF + FLG) % 31 ) == 0 ) {

		int FDICT = ( FLG >> 5 ) & 1;
		int FLEVEL = FLG >> 6;

		bool lastBlock = false;
		while ( !lastBlock ) {

			int BFINAL = getNBits( data + index, bit, 1 );
			bit += 1; if ( bit > 7 ) { index += bit / 8; bit &= 7; }

			if ( BFINAL ) { lastBlock = true; }

			int BTYPE = getNBits( data + index, bit, 2 );
			bit += 2; if ( bit > 7 ) { index += bit / 8; bit &= 7; } 

			int position[3] = { bit, index, outIndex };
			if ( BTYPE == 0 ) {
				decodeNonCompressedBlock( data, position, outData );
			}
			else if ( BTYPE == 1 ) {
				//decodeBlockHuffmanFixed( data + index + 2, position );
			}
			else if ( BTYPE == 2 ) {
				decodeBlockHuffmanDynamic( data, position, outData );
			}

			bit = position[0];
			index = position[1];
			outIndex = position[2];
		}
	}

	if ( colorType == 2 ) {
		displayPNGColorType2( width, height, outData, pixels );
		delete outData;

		screenData * imageData = new screenData;
		imageData -> screen = pixels;
		imageData -> windowWidth = width;
		imageData -> windowHeight = height;

		return imageData;
	}

	else if ( colorType == 6 ) {
		displayPNGColorType6( width, height, outData, pixels );
		delete outData;

		screenData * imageData = new screenData;
		imageData -> screen = pixels;
		imageData -> windowWidth = width;
		imageData -> windowHeight = height;

		return imageData;
	}
	return NULL;
}





