#ifndef _EXTENSIONS___DATA_OPERATIONS_H_
#define _EXTENSIONS___DATA_OPERATIONS_H_

extern int andValues[];

extern int bitReversals[];

struct huffmanTable {
	int * table1;
	int * table2;
};

bool compareData( unsigned char * file1, unsigned char * file2, int len );

int parseData( unsigned char * string, unsigned char * search, int len1, int len2 );

bool compareString( char * s1, char * s2 );

void displayBinary( int x, int digits );

void printBlock( int * data );

int flipWord( int value, int bitshift );

int getDwordBE( unsigned char * string, int index );

void deleteHuffTable( huffmanTable * table );

int fillHuffmanTable( huffmanTable * table, unsigned char * pointer, unsigned char * codePointer );

int fillHuffTableIntSource( huffmanTable * table, int * pointer, int * codePointer );

#endif // _EXTENSIONS___DATA_OPERATIONS_H_