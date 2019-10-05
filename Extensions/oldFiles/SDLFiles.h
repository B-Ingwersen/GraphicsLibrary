#include <fstream>
#include <iostream>
#include <stdio.h>

long loopRepeats = 0;

struct fileData {
	unsigned char * buffer;
	int fileSize;
};

fileData * openFileToBuffer( char * fileName ) {
	FILE * file;
	file = fopen( fileName, "r" );

	fseek( file, 0L, SEEK_END);
	int fileSize = ftell( file );

	unsigned char * buffer = new unsigned char[ fileSize ];
	fileData * fileInfo = new fileData;
	fileInfo -> buffer = buffer;
	fileInfo -> fileSize = fileSize;

	rewind( file );
	fread( buffer, fileSize, 1, file );
	fclose( file );

	return fileInfo;
}

bool compareData( unsigned char * file1, unsigned char * file2, int len ) {
	int i;
	for ( i = 0; i < len; i++ ) {
		if ( file1[ i ] != file2[ i ] ) { return false; }
	}
	return true;
}

int parseData( unsigned char * string, unsigned char * search, int len1, int len2 ) {
	int i;
	for ( i = 0; i < len2 - len1; i++ ) {
		if ( compareData( string, search + i, len1 ) ) { return i; }
	}
	return -1;
}

unsigned char PNGHeader[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
unsigned char JPEGMagicNumber[] = { 0xFF, 0xD8 };
uint32_t * renderPNGImage( unsigned char * data, int fileSize );
screenData * renderJPEGImage( unsigned char * data, int fileSize );

screenData * openPicture( char * fileName ) {

	fileData * file = openFileToBuffer( fileName );
	unsigned char * dataBuffer = file -> buffer;
	int fileSize = file -> fileSize;

	//cout << "File length in bytes: " << fileSize << endl;

	if ( compareData( dataBuffer, PNGHeader, 8 ) ) {
		//cout << "It is a PNG image" << endl;

		renderPNGImage( dataBuffer, fileSize );
	}

	if (compareData( dataBuffer, JPEGMagicNumber, 2) ) {
		//cout << "It is a JPEG image" << endl;

		return renderJPEGImage( dataBuffer, fileSize );
	}
	return NULL;
}

struct huffmanTable {
	int * table;
	int * tableValues;
	int * tableIndexes;
	int * tableIndexLengths;
	int * table2;
};

void displayBinary( int x, int digits ) {
	int i;
	for ( i = 0; i < digits; i++ ) {
		cout << ( ( x >> ( digits - i - 1 ) ) & 1 );
	}
}

int fillHuffmanTable( huffmanTable * table, unsigned char * pointer ) {
	unsigned char * codePointer = pointer + 16;
	int i, j;
	int numberOfEntries = 0;
	for ( i = 0; i < 16; i++ ) {
		numberOfEntries += pointer[ i ];
	}

	int * expandedTable = new int[ numberOfEntries ];
	int * expandedTableValues = new int[ numberOfEntries ];
	int * expandedTableIndexes = new int[ 16 ];
	int * expandedTableIndexLengths = new int [ 16 ];
	int index = 0;
	int value = 0;
	for ( i = 0; i < 16; i++ ) {
		expandedTableIndexes[ i ] = index;
		for ( j = 0; j < pointer[ i ]; j++ ) {
			expandedTable[ index ] = value;
			expandedTableValues[ index ] = codePointer[ index ];
			//cout << i + 1 << "\t"; displayBinary( value, 16 ); cout << ":\t" << expandedTableValues[ index ] << endl;
			index++;
			value++;
		}
		expandedTableIndexLengths[ i ] = j;
		value = value << 1;
	}

	table -> table = expandedTable;
	table -> tableValues = expandedTableValues;
	table -> tableIndexes = expandedTableIndexes;
	table -> tableIndexLengths = expandedTableIndexLengths;
	
	return numberOfEntries;
}

void fillHuffmanTable2( huffmanTable * table, unsigned char * pointer ) {
	unsigned char * codePointer = pointer + 16;
	int i, j;
	int numberOfEntries = 0;
	int maxIndex = 0;
	for ( i = 0; i < 16; i++ ) {
		numberOfEntries += pointer[ i ];
		if ( pointer[i] > 0 ) {
			maxIndex = i + 1;
		}
	}

	int * expandedTable = new int[ 1024 ];
	expandedTable[0] = 2;
	expandedTable[1] = 4;

	int index = 0;
	int tableIndex = 2;
	int value = 0;
	int branches = 2;
	for ( i = 0; i < maxIndex; i++ ) {
		branches -= pointer[i];
		for ( j = 0; j < pointer[ i ]; j++ ) {
			expandedTable[ tableIndex ] = -codePointer[ index ];
			expandedTable[ tableIndex + 1] = -codePointer[ index ];
			//cout << tableIndex << ":\t" << expandedTable[ tableIndex ] << ",\t" << expandedTable[ tableIndex + 1] << endl;

			tableIndex += 2;
			index++;
		}

		int postBranchIndex = tableIndex + 2 * branches;
		for ( j = 0; j < branches; j++ ) {
			expandedTable[ tableIndex ] = postBranchIndex + 4 * j;
			expandedTable[ tableIndex + 1 ] = postBranchIndex + 4 * j + 2;
			//cout << tableIndex << ":\t" << expandedTable[ tableIndex ] << ",\t" << expandedTable[ tableIndex + 1] << endl;
			tableIndex += 2;
		}
		branches *= 2;	
	}

	table -> table2 = expandedTable;

}

void copyQuantizationTable( int * destination, unsigned char * source ) {
	int i;
	for ( i = 0; i < 64; i++ ) {
		destination[i] = source[i];
	}
}

int getFirstNBits[16] = { 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535 };
int DCPositiveBases[12] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
int DCNegativeBases[12] = { 0, -1, -3, -7, -15, -31, -63, -127, -255, -511, -1023, -2047 };

int zigZagIndexes[] = {	0,	1,	8, 	16,	9,	2,	3,	10,
			17,	24,	32,	25,	18,	11,	4,	5,
			12,	19,	26,	33,	40,	48,	41,	34,
			27,	20,	13,	6,	7,	14,	21,	28,
			35,	42,	49,	56,	57,	50,	43,	36,
			29,	22,	15,	23,	30,	37,	44,	51,
			58,	59,	52,	45,	38,	31,	39,	46,
			53,	60,	61,	54,	47,	55,	62,	63	};

#define PI 3.14159
#define invSQRT2 0.7071068

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

int andValues[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535 };

unsigned int getNBits( unsigned char * data, int bit, int len ) {
	if ( len == 0 ) { return 0; }

	/*unsigned char copy[4];
	copy[3] = 0;
	copy[2] = data[0] & andValues[ 8 - bit ];
	copy[1] = data[1];
	copy[0] = data[2];
	unsigned int value = *(unsigned int*)(copy) >> (24 - bit - len);*/

	unsigned int value = ( ((data[0] & andValues[8 - bit])<< 16) + (data[1] << 8) + data[2] );

	return ( value >> ( 24 - bit - len ) );
}

unsigned int getNBits2( unsigned char * data, int bit, int len ) {
	if ( len == 0 ) { return 0; }

	unsigned int value = ( ( data[0] << 8 ) + data[1] ) & andValues[ 16 - bit ];

	return ( value >> ( 16 - bit - len ) );
}

/*int getTableValuesDC( unsigned char * pointer, int * position, huffmanTable * table ) {

	int bit = position[ 0 ];
	int index = position[ 1 ];
	int indexLength, tableIndex, i, DCBitValue;

	int value = 0;

	bool foundInTable = false;
	int bits = 0;
	while ( !foundInTable ) {
		value <<= 1; value |= ( pointer[ index ] >> ( 7 - bit ) ) & 1;
		bit++; if ( bit > 7 ) { bit = 0; index++; }

		indexLength = table -> tableIndexLengths[ bits ];
		tableIndex = table -> tableIndexes[ bits ];
		for ( i = 0; i < indexLength; i++ ) {
			if ( table -> table[ tableIndex + i ] == value ) {
				DCBitValue = table -> tableValues[ tableIndex + i ];
				foundInTable = true;
				break;
			}
		}
		bits++;
	}

	if ( DCBitValue == 0 ) { 
		position[ 0 ] = bit;
		position[ 1 ] = index;
		return 0;
	}
	else {
		bool DCSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;

		value = getNBits( pointer + index, bit + 1, DCBitValue - 1 );
		bit += DCBitValue; if ( bit > 7 ) { index += bit >> 3; bit %= 8; }

		position[ 0 ] = bit;
		position[ 1 ] = index;

		if ( DCSign ) { return DCPositiveBases[ DCBitValue ] + value; }
		else { return DCNegativeBases[ DCBitValue ] + value; }
	}
}*/

int getTableValuesDC2( unsigned char * pointer, int * position, huffmanTable * huffmanTable ) {

	int bit = position[ 0 ];
	int index = position[ 1 ];
	int i, DCBitValue;

	int * table = huffmanTable -> table2;

	int value = 0;
	int tableIndex = 0;
	int indexValue;
	while ( true ) {
		value = ( pointer[ index ] >> ( 7 - bit ) ) & 1;

		tableIndex = table[ tableIndex + value ];
		if ( tableIndex <= 0 ) {
			DCBitValue = -tableIndex;
			break;
		}

		bit++; if ( bit > 7 ) { bit = 0; index++; }
	}

	if ( DCBitValue == 0 ) { 
		position[ 0 ] = bit;
		position[ 1 ] = index;
		return 0;
	}
	else {
		/*bool DCSign = false;
		value = 0;
		if ( DCBitValue ) {
			DCSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;
			bit++; if ( bit > 7 ) { bit = 0; index++; }
			for ( i = 0; i < DCBitValue - 1; i++ ) {
				value <<= 1; value += (( pointer[ index ] >> ( 7 - bit ) ) & 1);
				bit++; if ( bit > 7 ) { bit = 0; index++; }
			}
		}*/

		bool DCSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;
		value = getNBits( pointer + index, bit + 1, DCBitValue - 1 );
		bit += DCBitValue; if ( bit > 7 ) { index += bit >> 3; bit %= 8; }

		position[ 0 ] = bit;
		position[ 1 ] = index;

		if ( DCSign ) { return DCPositiveBases[ DCBitValue ] + value; }
		else { return DCNegativeBases[ DCBitValue ] + value; }
	}
}

int getTableValuesDC3( unsigned char * pointer, int * position, huffmanTable * huffmanTable ) {

	int bit = position[ 0 ];
	int index = position[ 1 ];
	int i, DCBitValue;

	int * table = huffmanTable -> table2;

	int value = 0;
	int tableIndex = 0;
	int indexValue;
	while ( true ) {
		value = ( pointer[ index ] >> bit ) & 1;

		tableIndex = table[ tableIndex + value ];
		if ( tableIndex <= 0 ) {
			DCBitValue = -tableIndex;
			break;
		}

		bit--; if ( bit < 0 ) { bit = 7; index++; }
	}

	if ( DCBitValue == 0 ) { 
		position[ 0 ] = bit;
		position[ 1 ] = index;
		return 0;
	}
	else {
		bool DCSign = false;
		value = 0;
		if ( DCBitValue ) {
			DCSign = ( pointer[ index ] >> bit ) & 1;
			bit--; if ( bit < 0 ) { bit = 7; index++; }
			for ( i = 0; i < DCBitValue - 1; i++ ) {
				value <<= 1; value += (( pointer[ index ] >> bit ) & 1);
				bit--; if ( bit < 0 ) { bit = 7; index++; }
			}
		}

		position[ 0 ] = bit;
		position[ 1 ] = index;

		if ( DCSign ) { return DCPositiveBases[ DCBitValue ] + value; }
		else { return DCNegativeBases[ DCBitValue ] + value; }
	}
}

/*void getTableValuesAC( unsigned char * pointer, int * position, huffmanTable * huffmanTable, int * qtTable, int * storeData ) {
	int * table = huffmanTable -> table;
	int * tableValues = huffmanTable -> tableValues;
	int * tableIndexes = huffmanTable -> tableIndexes;
	int * tableIndexLengths = huffmanTable -> tableIndexLengths;

	int bit = position[ 0 ];
	int index = position[ 1 ];
	bool foundInTable;
	int bits, value, i, indexLength, tableIndex, ACBitValue;
	int storeIndex = 1;

	
	bool endOfBlock = false;
	while ( !endOfBlock ) {
		value = 0;

		foundInTable = false;
		bits = 0;
		while ( !foundInTable ) {
			value <<= 1; value |= ( pointer[ index ] >> ( 7 - bit ) ) & 1;
			bit++; if ( bit > 7 ) { bit = 0; index++; }

			indexLength = tableIndexLengths[ bits ];
			tableIndex = tableIndexes[ bits ];
			for ( i = 0; i < indexLength; i++ ) {
				if ( table[ tableIndex + i ] == value ) {
					ACBitValue = tableValues[ tableIndex + i ];

					if ( ACBitValue == 0 ) {
						position[ 0 ] = bit;
						position[ 1 ] = index;
						return;
					}

					storeIndex += ACBitValue >> 4;
					ACBitValue &= 0x0F;
					foundInTable = true;
					break;
				}
			}
			bits++;
		}

		value = 0;
		bool ACSign;
		ACSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;
		value = getNBits( pointer + index, bit + 1, ACBitValue - 1 );
		bit += ACBitValue; if ( bit > 7 ) { index += bit >> 3; bit %= 8; }

		if ( ACSign ) { storeData[ zigZagIndexes[ storeIndex ] ] = ( DCPositiveBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		else { 		storeData[ zigZagIndexes[ storeIndex ] ] = ( DCNegativeBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		storeIndex++;
		if ( storeIndex > 63 ) {
			position[ 0 ] = bit;
			position[ 1 ] = index;
			return;
		}
	}
}*/

/*void getTableValuesAC2( unsigned char * pointer, int * position, huffmanTable * huffmanTable, int * qtTable, int * storeData ) {
	int * table = huffmanTable -> table2;

	int bit = position[ 0 ];
	int index = position[ 1 ];

	int i, ACBitValue, indexValue, tableIndex, value;
	int storeIndex = 1;

	bool endOfBlock = false;
	while ( !endOfBlock ) {

		value = 0;
		tableIndex = 0;
		while ( true ) {
			value = ( pointer[ index ] >> ( 7 - bit ) ) & 1;

			tableIndex = table[ tableIndex + value ];
			if ( tableIndex <= 0 ) {
				ACBitValue = -tableIndex;

				if ( ACBitValue == 0 ) {
					position[ 0 ] = bit;
					position[ 1 ] = index;
					return;
				}

				storeIndex += ACBitValue >> 4;
				ACBitValue &= 0x0F;
				break;
			}

			bit++; if ( bit > 7 ) { bit = 0; index++; }
		}

		bool ACSign = false;
		value = 0;
		if ( ACBitValue ) {
			ACSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;
			bit++; if ( bit > 7 ) { bit = 0; index++; }
			for ( i = 0; i < ACBitValue - 1; i++ ) {
				value <<= 1; value += (( pointer[ index ] >> ( 7 - bit ) ) & 1);
				bit++; if ( bit > 7 ) { bit = 0; index++; }
			}
		}

		if ( ACSign ) { storeData[ zigZagIndexes[ storeIndex ] ] = ( DCPositiveBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		else { 		storeData[ zigZagIndexes[ storeIndex ] ] = ( DCNegativeBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		storeIndex++;
		if ( storeIndex > 63 ) {
			position[ 0 ] = bit;
			position[ 1 ] = index;
			return;
		}
	}
}*/

void getTableValuesAC3( unsigned char * pointer, int * position, huffmanTable * huffmanTable, int * qtTable, int * storeData ) {
	int * table = huffmanTable -> table2;

	int bit = position[ 0 ];
	int index = position[ 1 ];

	int i, ACBitValue, indexValue, tableIndex, value;
	int storeIndex = 1;

	bool endOfBlock = false;
	while ( !endOfBlock ) {

		value = 0;
		tableIndex = 0;
		while ( true ) {
			value = ( pointer[ index ] >> bit ) & 1;

			tableIndex = table[ tableIndex + value ];
			if ( tableIndex <= 0 ) {
				ACBitValue = -tableIndex;

				if ( ACBitValue == 0 ) {
					position[ 0 ] = bit;
					position[ 1 ] = index;
					return;
				}

				storeIndex += ACBitValue >> 4;
				ACBitValue &= 0x0F;
				break;
			}

			bit--; if ( bit < 0 ) { bit = 7; index++; }
		}

		bool ACSign = false;
		value = 0;
		if ( ACBitValue ) {
			ACSign = ( pointer[ index ] >> bit ) & 1;
			bit--; if ( bit < 0 ) { bit = 7; index++; }
			for ( i = 0; i < ACBitValue - 1; i++ ) {
				value <<= 1; value += (( pointer[ index ] >> bit ) & 1);
				bit--; if ( bit < 0 ) { bit = 7; index++; }
			}
		}

		if ( ACSign ) { storeData[ zigZagIndexes[ storeIndex ] ] = ( DCPositiveBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		else { 		storeData[ zigZagIndexes[ storeIndex ] ] = ( DCNegativeBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		storeIndex++;
		if ( storeIndex > 63 ) {
			position[ 0 ] = bit;
			position[ 1 ] = index;
			return;
		}
	}
}

void getTableValuesAC4( unsigned char * pointer, int * position, huffmanTable * huffmanTable, int * qtTable, int * storeData ) {
	int * table = huffmanTable -> table2;

	int bit = position[ 0 ];
	int index = position[ 1 ];

	int i, ACBitValue, indexValue, tableIndex, value;
	int storeIndex = 1;

	bool endOfBlock = false;
	while ( !endOfBlock ) {

		value = 0;
		tableIndex = 0;
		while ( true ) {
			value = ( pointer[ index ] >> ( 7 - bit ) ) & 1;

			tableIndex = table[ tableIndex + value ];
			if ( tableIndex <= 0 ) {
				ACBitValue = -tableIndex;

				if ( ACBitValue == 0 ) {
					position[ 0 ] = bit;
					position[ 1 ] = index;
					return;
				}

				storeIndex += ACBitValue >> 4;
				ACBitValue &= 0x0F;
				break;
			}

			bit++; if ( bit > 7 ) { bit = 0; index++; }
		}

		value = 0;
		bool ACSign;
		ACSign = ( pointer[ index ] >> ( 7 - bit ) ) & 1;
		value = getNBits2( pointer + index, bit + 1, ACBitValue - 1 );
		bit += ACBitValue; if ( bit > 7 ) { index += bit >> 3; bit %= 8; }

		if ( ACSign ) { storeData[ zigZagIndexes[ storeIndex ] ] = ( DCPositiveBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		else { 		storeData[ zigZagIndexes[ storeIndex ] ] = ( DCNegativeBases[ ACBitValue ] + value ) * qtTable[ storeIndex ]; }
		storeIndex++;
		if ( storeIndex > 63 ) {
			position[ 0 ] = bit;
			position[ 1 ] = index;
			return;
		}
	}
}

void printBlock( int * data ) {
	int i, j;
	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			cout << "\t" << data[ 8 * i + j ];
		}
		cout << endl;
	}
}

float IDCTTable[ 64 ][ 64 ];
void generateIDCTTable() {
	int x, y;
	for ( x = 0; x < 8; x++ ) {
		for ( y = 0; y < 8; y++ ) {
			float value;
			int u, v;
			for ( u = 0; u < 8; u++ ) {
				for ( v = 0; v < 8; v++ ) {
					value = cos((2*x + 1) * u * PI / 16) * cos((2*y + 1) * v * PI / 16);
					if ( u == 0 ) { value *= invSQRT2; }
					if ( v == 0 ) { value *= invSQRT2; }
					IDCTTable[8 * y + x][8 * v + u] = value / 4;
				}
			}
		}
	}
}

void inverseDCT( int * data, int * result ) {
	int x, y;
	for ( x = 0; x < 8; x++ ) {
		for ( y = 0; y < 8; y++ ) {
			float sum = 0; float subsum;
			int u, v;
			for ( u = 0; u < 8; u++ ) {
				for ( v = 0; v < 8; v++ ) {
					subsum = data[ 8 * v + u ] * cos((2*x + 1) * u * PI / 16) * cos((2*y + 1) * v * PI / 16);
					if ( u == 0 ) { subsum *= invSQRT2; }
					if ( v == 0 ) { subsum *= invSQRT2; }
					sum += subsum;
				}
			}
			result[ 8 * y + x ] = round( sum / 4 );
		}
	}
	printBlock( result );
}

void inverseDCTWithTable( int * data, int * result ) {
	int i, j;
	for ( i = 0; i < 64; i++ ) {
		float sum = 0;
		for ( j = 0; j < 64; j++ ) {
			sum += data[j] * IDCTTable[i][j];
		}
		result[i] = round( sum );
	}
}

void inverseDCTWithTableWithAdd( int * data, int * result ) {
	int i, j;
	for ( i = 0; i < 64; i++ ) {
		float sum = 0;
		for ( j = 0; j < 64; j++ ) {
			sum += data[j] * IDCTTable[i][j];
		}
		result[i] = round( sum ) + 128;
	}
}

void blockIntToFloat( int * data, float * result ) {
	int i;
	for ( i = 0; i < 64; i++ ) {
		result[i] = data[i];
	}
}

void blockFloatToInt( float * data, int * result ) {
	int i;
	for ( i = 0; i < 64; i++ ) {
		result[i] = round(data[i]);
	}
}

float IDCTTable1D[8][8];
void generate1DTable() {
	int i, j;
	float value;
	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < 8; j++ ) {
			value = cos((2*i + 1) * j * PI / 16) / 2;
			if ( j == 0 ) { value *= invSQRT2; }
			IDCTTable1D[i][j] = value;
		}
	}
};

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

void idct(int *b) // 2D 8x8 IDCT
{
	int i, b2[64];
	for (i=0; i<8; i++) idct1(b+i*8, b2+i, 9, 1<<8); // row
	for (i=0; i<8; i++) idct1(b2+i*8, b+i, 12, 1<<11); // col 
}

void idct128(int *b) // 2D 8x8 IDCT
{
	int i, b2[64];
	for (i=0; i<8; i++) idct1(b+i*8, b2+i, 9, 1<<8); // row
	for (i=0; i<8; i++) idct1(b2+i*8, b+i, 12, 1<<11); // col 
	for ( i = 0; i < 64; i++ ) { b[i] += 128; }
}

void IDCT1D( float * data, float * result ) {
	int x = 0;
	for ( x = 0; x < 8; x++ ) {
		int i;
		float sum = 0;
		for ( i = 0; i < 8; i++ ) {
			sum += data[i] * IDCTTable1D[x][i];
		}
		result[x] = sum;
		cout << sum << ",\t";
	}
	cout << endl;
}

void IDCT1DH( float * data, float * result ) {
	int x = 0;
	int i;
	float sum;
	for ( x = 0; x < 8; x++ ) {
		sum = 0;
		for ( i = 0; i < 8; i++ ) {
			sum += data[8 * i] * IDCTTable1D[x][i];
		}
		result[8 * x] = sum;
	}
}

void IDCTSeperated( int * data, int * result ) {
	float float1[64];
	float float2[64];
	blockIntToFloat( data, float1 );

	IDCT1D( float1 + 0, float2 + 0 );
	IDCT1D( float1 + 8, float2 + 8 );
	IDCT1D( float1 + 16, float2 + 16 );
	IDCT1D( float1 + 24, float2 + 24 );
	IDCT1D( float1 + 32, float2 + 32 );
	IDCT1D( float1 + 40, float2 + 40 );
	IDCT1D( float1 + 48, float2 + 48 );
	IDCT1D( float1 + 56, float2 + 56 );

	IDCT1DH( float2, float1 );
	IDCT1DH( float2 + 1, float1 + 1);
	IDCT1DH( float2 + 2, float1 + 2);
	IDCT1DH( float2 + 3, float1 + 3);
	IDCT1DH( float2 + 4, float1 + 4);
	IDCT1DH( float2 + 5, float1 + 5);
	IDCT1DH( float2 + 6, float1 + 6);
	IDCT1DH( float2 + 7, float1 + 7);

	blockFloatToInt( float1, result );

	//printBlock( result );
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


unsigned char JPEGMarker[] = { 0xFF, 0xDA };
unsigned char JPEGFalseMarker[] = { 0xFF, 0x00 };
unsigned char JFIFMarker[] = "JFIF";

int getWordBE( unsigned char * data ) {
	return ((int)(* data )) * 256 + ((int)(*( data + 1)));
}

void print444Macroblocks( unsigned char * data, int ** quantPointers, huffmanTable ** huffPointers, screenData * imageData, int * position, int width, int height, int blocks ) {
	//int position[2] = { 0, 0 };

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
	cout << blocks << endl;
	for ( block = 0; block < blocks; block++ ) {

		memset( YData, 0, 256 );
		Y_DC += getTableValuesDC2( data, position, YDC );
		YData[0] = Y_DC * YQuant[0];
		getTableValuesAC4( data, position, YAC, YQuant, YData );
		idct128( YData );
		//cout << "Y:"; printBlock( YData );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC2( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC4( data, position, CbAC, CrQuant, CbData );
		idct( CbData );
		//cout << "Cb:"; printBlock( CbData );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC2( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC4( data, position, CrAC, CrQuant, CrData );
		idct( CrData );
		//cout << "Cr:"; printBlock( CrData );

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
		Y_DC += getTableValuesDC2( data, position, YDC );
		Y00Data[0] = Y_DC * YQuant[0];
		getTableValuesAC4( data, position, YAC, YQuant, Y00Data );
		idct128( Y00Data );	//inverseDCTWithTableWithAdd( Y00Data, Y00Final );
		//cout << "Y00:"; printBlock( Y00Final );

		memset( Y10Data, 0, 256 );
		Y_DC += getTableValuesDC2( data, position, YDC );
		Y10Data[0] = Y_DC * YQuant[0];
		getTableValuesAC4( data, position, YAC, YQuant, Y10Data );
		idct128( Y10Data );	//inverseDCTWithTableWithAdd( Y10Data, Y10Final );
		//cout << "Y10:"; printBlock( Y10Final );

		memset( Y01Data, 0, 256 );
		Y_DC += getTableValuesDC2( data, position, YDC );
		Y01Data[0] = Y_DC * YQuant[0];
		getTableValuesAC4( data, position, YAC, YQuant, Y01Data );
		idct128( Y01Data );
		//cout << "Y01:"; printBlock( Y01Final );

		memset( Y11Data, 0, 256 );
		Y_DC += getTableValuesDC2( data, position, YDC );
		Y11Data[0] = Y_DC * YQuant[0];
		getTableValuesAC4( data, position, YAC, YQuant, Y11Data );
		idct128( Y11Data );
		//cout << "Y11:"; printBlock( Y11Final );

		memset( CbData, 0, 256 );
		Cb_DC += getTableValuesDC2( data, position, CbDC );
		CbData[0] = Cb_DC * CbQuant[0];
		getTableValuesAC4( data, position, CbAC, CrQuant, CbData );
		idct( CbData );
		//cout << "Cb:"; printBlock( CbFinal );

		memset( CrData, 0, 256 );
		Cr_DC += getTableValuesDC2( data, position, CrDC );
		CrData[0] = Cr_DC * CrQuant[0];
		getTableValuesAC4( data, position, CrAC, CrQuant, CrData );
		idct( CrData );
		//cout << "Cr:"; printBlock( CrFinal );

		int x, y;
		int k = 0;
		int l;
		int pixelIndex = width * j + i;
		for ( y = 0; y < 8; y++ ) {
			for ( x = 0; x < 8; x++ ) {
				l = 8 * (y >> 1) + x / 2;
				//cout << l << endl;
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

	huffmanTable componentTables[4][2];
	screenData * imageData;
	int position[] = { 0, 0, 0 };
	int restartInterval = 0;

	int renderWidth;
	int renderHeight;

	//long totalMemoryCopied = 0;

	int index = 0;
	while ( !reachedEnd ) {
		index += 2;
		index += parseData( JPEGMarker, data + index, 1, fileSize );

		if ( *( data + index + 1) == 192 || ( *( data + index + 1) == 0xC2 ) ) {
			cout << "SOF0 @ " << index << endl;
			int length = getWordBE( data + index + 2);	cout << "length is:\t" << length << endl;
			dataPrecision = *( data + index + 4); 		cout << "Data precision:\t" << dataPrecision << endl;
			height = getWordBE( data + index + 5);		cout << "Image height:\t" << height << endl;
			width = getWordBE( data + index + 7); 		cout << "Image width:\t" << width << endl;
			numberOfComponents = *( data + index + 9 ); 	cout << "#ofComponents:\t" << numberOfComponents << endl;

			int i;
			for ( i = 0; i < numberOfComponents; i++ ) {
				int compID = (int)( data[ index + 10 + 3 * i ] ); cout << "Component ID:\t" << compID << endl;
				int sample = (int)( data[ index + 10 + 3 * i + 1] ); cout << "\tSamp:\t" << sample << endl;
				int quant = (int)( data[ index + 10 + 3 * i + 2] ); cout << "\tQuant:\t" << quant << endl;
				if ( compID == 1 ) { YSample = sample; 	YQuant = qTables[ quant ]; }
				if ( compID == 2 ) { CbSample = sample;	CbQuant = qTables[ quant ]; }
				if ( compID == 3 ) { CrSample = sample; CrQuant = qTables[ quant ]; }
			}

			renderWidth = width;
			renderHeight = height;

			if ( YSample == 0x11 ) {
				if ( CbSample != 0x11 || CrSample != 0x11 ) { return NULL; }
				samplingFactor = 444;
				renderWidth = width;	if ( width % 8 > 0 ) { renderWidth += 8 - width % 8; }
				renderHeight = height;	if ( height % 8 > 0 ) { renderHeight += 8 - height % 8; }
				if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 64; }
			}
			if ( YSample == 0x22 ) {
				if ( CbSample == 0x21 ) {
					if ( CrSample != 0x21 ) { return NULL; }
					samplingFactor = 422;
					renderWidth = width;	if ( width % 16 > 0 ) { renderWidth += 16 - width % 16; }
					renderHeight = height;	if ( height % 8 > 0 ) { renderHeight += 8 - height % 8; }
					if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 128; }
				}
				if ( CbSample == 0x11 ) {
					if ( CrSample != 0x11 ) { return NULL; }
					samplingFactor = 420;
					renderWidth = width;	if ( width % 16 > 0 ) { renderWidth += 16 - width % 16; }
					renderHeight = height;	if ( height % 16 > 0 ) { renderHeight += 16 - height % 16; }
					if ( restartInterval == 0 ) { restartInterval = renderWidth * renderHeight / 256; cout << restartInterval << endl; }	
				}
			}

			uint32_t * pixels = new uint32_t[ renderWidth * renderHeight ];
			imageData = new screenData;
			imageData -> screen = pixels;
			imageData -> windowWidth = renderWidth;
			imageData -> windowHeight = renderHeight;

			cout << endl;
		}

		//else if ( *( data + index + 1) == 0xE0 ) {
			//cout << "APP0 @ " << index << endl;
			//int length = (*( data + index + 2)) * 256 + (*( data + index + 3));
			//if ( compareData( data + index + 4, JFIFMarker, 5) ) { /*cout << "Is JFIF Format" << endl;*/ }

			/*int densityUnits = (*( data + index + 11)); //cout << "Density units:\t" << densityUnits << endl;
			int xDensity = ((int)(*( data + index + 12))) * 256 + ((int)(*( data + index + 13)));
				//cout << "xDensity: \t" << xDensity << endl;
			int yDensity = ((int)(*( data + index + 14))) * 256 + ((int)(*( data + index + 15)));
				//cout << "yDensity: \t" << yDensity << endl;
			int xThumbnail = *(data + index + 16); //cout << "xThumbnail:\t" << xThumbnail << endl;
			int yThumbnail = *(data + index + 16); //cout << "yThumbnail:\t" << yThumbnail << endl;

			//cout << endl;
		}*/

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

				if ( tableClass ) {
					fillHuffmanTable2( &huffmanTablesAC[ tableDest ], data + index + subIndex + 5 );
					subIndex += fillHuffmanTable( &huffmanTablesAC[ tableDest ], data + index + subIndex + 5 );
				}
				else {
					fillHuffmanTable2( &huffmanTablesDC[ tableDest ], data + index + subIndex + 5 );
					subIndex += fillHuffmanTable( &huffmanTablesDC[ tableDest ], data + index + subIndex + 5 );
				}

				subIndex += 17;
			}
			//cout << index + subIndex + 5 << endl;

			cout << endl;
		}
		else if ( *( data + index + 1) == 0xDB ) {
			cout << "DQT @ " << index << endl;
			int length = getWordBE( data + index + 2); //cout << "length is:\t" << length << endl;

			numberOfQT = (length - 2) / 65;

			int i;
			for ( i = 0; i < numberOfQT; i++ ) {
				int qtInfo = *(data + index + 4 + 65 * i); 		cout << "QT information:\t" << qtInfo << endl;
				int elementPrecision = (qtInfo & 0xF0) >> 4;		cout << "\teP:\t" << elementPrecision << endl;
					if ( elementPrecision ) { return NULL; } // will only work for 8 bit precision
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

			int i;
			for ( i = 0; i < components; i++ ) {
				int componentID = *(data + index + 5 + 2 * i); 	cout << " \tCID:\t" << componentID << endl;
				int huffmanTblID = *(data + index + 6 + 2 * i); cout << " \tHTbl:\t" << huffmanTblID << endl;
				int acTable = huffmanTblID & 0x0F;
				int dcTable = (huffmanTblID & 0xF0) >> 4;
				/*componentTables[ componentID][ acTable ] = huffmanTablesAC[ componentID ];
				componentTables[ componentID][ dcTable ] = huffmanTablesDC[ componentID ];*/
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
				print444Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			else if ( samplingFactor == 420 ) {	
				print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			//cout << endl;

			index -= 2;
		}

		else if ( *( data + index + 1) >= 0xD0 && *( data + index + 1) < 0xD8 ) {
			//cout << "Marker of type " << (int)( *(data + index + 1 ) ) << " @ " << index << endl;
			//cout << (int)(data[ index + 2]) << ", " << (int)(data[ index + 3]) << endl;

			index += 2;
			SOSindex = index;
			cout << "SOS @" << SOSindex << endl;

			int copyIndex = SOSindex;
			int restOfFile = fileSize - SOSindex;

			bool keepParsing = true;

			int i = parseData( JPEGFalseMarker, data + index, 1, restOfFile );
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
				position[0] = 0;
				position[1] = 0;
				print444Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			else if ( samplingFactor == 420 ) {
				position[0] = 0;
				position[1] = 0;
				print420Macroblocks( data + SOSindex, quantPointers, huffPointers, imageData, position, renderWidth, renderHeight, restartInterval );
			}

			//cout << endl;

			index -= 2;

		}

	}

	//cout << "Starting scans @ " << SOSindex << endl;
	return imageData;
}














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

uint32_t * renderPNGImage( unsigned char * data, int fileSize ) {
	data += 8;
	int pointer = 8;

	int width, height;
	int bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;

	while ( pointer < fileSize ) {
		int length = readIntBigEndian( data);
		data += 8;

		if ( compareData( data - 4, IHDR, 4 ) ) {
			width = readIntBigEndian( data );
			height = readIntBigEndian( data + 4 );
			cout << "Image dimensions are: " << width << "x" << height << endl;

			bitDepth = *(data + 8);
			colorType = *(data + 9);
			cout << "Color type: " << colorType << endl;
			compressionMethod = *(data + 10);
			cout << "Compression method: " << compressionMethod << endl;
			filterMethod = *(data + 11);
			interlaceMethod = *(data + 12);
		}
		else if ( compareData( data - 4, PLTE, 4 ) ) {
			cout << "Number of colors " << length;
		}
		else if ( compareData( data - 4, IDAT, 4 ) ) {
			int CMF = (int) (*data);
			int firstByte = (int) (*data);
		}

		data += length;
		data += 4;
		pointer += length + 12;
	}
	cout << "finished" << endl;
}


















