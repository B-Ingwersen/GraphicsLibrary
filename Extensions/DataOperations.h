int andValues[] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767, 65535 };

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

bool compareString( char * s1, char * s2 ) {
	int i;
	for ( i = 0; s1[i] == s2[i] && s1[i] != 0 && s2[i] != 0; i++ ) {}
	if (s1[i] == 0 && s2[i] == 0) {return true;}
	return false;
}

void displayBinary( int x, int digits ) {
	int i;
	for ( i = 0; i < digits; i++ ) {
		cout << ( ( x >> ( digits - i - 1 ) ) & 1 );
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

int flipWord( int value, int bitshift ) {
	displayBinary( value, 16 ); //cout << "\t"; displayBinary( value << bitshift, 16 );  cout << endl;
	value << bitshift;
	int lower = bitReversals[ value & 0xFF ];
	int upper = bitReversals[ value >> 8 ];
	return ( upper << 8 ) + lower;
}

int getDwordBE( unsigned char * string, int index ) {
	int value = string[ index + 3 ] + ( string[ index + 2 ] << 8 ) + ( string[ index + 1 ] << 16 ) + ( string[ index ] << 24 );
	return value;
}


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

int fillHuffmanTable( huffmanTable * table, unsigned char * pointer, unsigned char * codePointer ) {
	int i, j;
	int numberOfEntries = 0;
	for ( i = 0; i < 16; i++ ) {
		numberOfEntries += pointer[ i ];
	}

	int * expandedTable1 = new int[ 512 ];

	//cout << expandedTable1 << endl;
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

int fillHuffTableIntSource( huffmanTable * table, int * pointer, int * codePointer ) {
	int i, j;
	int numberOfEntries = 0;
	for ( i = 0; i < 16; i++ ) {
		numberOfEntries += pointer[ i ];
	}

	int * expandedTable1 = new int[ 512 ];

	for ( i = 0; i < 512; i++ ) {
		expandedTable1[i] = -1;
	}


	int index = 0;
	int value = 0;
	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < pointer[ i ]; j++ ) {
			int newValue = bitReversals[ value ] >> (7 - i);
			int k;
			for ( k = 0; k < 256; k += ( 1 << (i + 1) ) ) {
				expandedTable1[ 2 * (newValue + k) ] = codePointer[ index ];
				expandedTable1[ 2 * (newValue + k) + 1 ] = i + 1;
			}
			index++;
			value++;
		}
		value <<= 1;
	}

	//cout << endl;

	int numOf16Tables = 0;
	for ( i = 0; i < 256; i++ ) {
		if ( expandedTable1[ 2 * i ] == -1 ) {
			expandedTable1[ 2 * i + 1 ] = numOf16Tables;
			numOf16Tables++;
		}
	}

	// Displays the table -- for debugging
	for ( i = 0; i < 256; i++ ) {		
		//displayBinary( i, 8 ); cout << "\t" << expandedTable1[ 2 * i ] << "\t" << expandedTable1[ 2 * i + 1 ] << endl;
	}

	int * expandedTable2 = new int[numOf16Tables * 512];
	for ( i = 0; i < numOf16Tables * 512; i++ ) {
		expandedTable2[i] = -1;
	}

	for ( i = 0; i < 8; i++ ) {
		for ( j = 0; j < pointer[ i + 8 ]; j++ ) {
			int newValue = bitReversals[ value & andValues[i + 1] ] >> (7 - i);
			int table1Value = bitReversals[ value >> (i + 1) ];
			//displayBinary( newValue, 8 ); cout << "\t"; displayBinary( table1Value, 8 ); cout << endl;
			int * t2table = expandedTable2 + expandedTable1[ 2 * table1Value + 1] * 512;

			int k;
			for ( k = 0; k < 256; k += (1 << (i + 1) ) ) {
				t2table[ ( (newValue & 0xFF) + k ) * 2 ] = codePointer[ index ];
				t2table[ ( (newValue & 0xFF) + k) * 2 + 1 ] = i + 1;
			}

			index++;
			value++;
		}
		value <<= 1;
	}

	for ( i = 0; i < 256; i++ ) {		
		//displayBinary( i, 8 ); cout << "\t" << expandedTable2[ 2 * i ] << "\t" << expandedTable2[ 2 * i + 1 ] << endl;
	}

	table -> table1 = expandedTable1;
	table -> table2 = expandedTable2;
	
	return numberOfEntries;
}