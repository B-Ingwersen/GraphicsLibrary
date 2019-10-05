#include <fstream>
#include <iostream>
#include <stdio.h>

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

unsigned char PNGHeader[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
Uint32 * renderPNG( unsigned char * data );

void openPicture( char * fileName ) {

	fileData * file = openFileToBuffer( fileName );
	unsigned char * dataBuffer = file -> buffer;
	int fileSize = file -> fileSize;

	//cout << "File length in bytes: " << fileSize << endl;

	if ( compareData( dataBuffer, PNGHeader, 8 ) ) {
		renderPNGImage( dataBuffer, fileSize );
	}
}

/*int readIntBigEndian( unsigned char * data ) {
	unsigned char value[4];
	value[0] = data[3];
	value[1] = data[2];
	value[2] = data[1];
	value[3] = data[0];
	return *(int *)(value);
}*/

unsigned char IHDR[] = "IHDR";

/*void renderPNGImage( unsigned char * data, int fileSize ) {
	data += 8;
	int pointer = 8;

	int width, height;
	int bitDepth, colorType, compressionMethod, filterMethod, interlaceMethod;

	int length;// = readIntBigEndian( data);
	data += 4;

	if ( compareData( data, IHDR, 4 ) ) {
		data += 4;
		//width = readIntBigEndian( data );
		//height = readIntBigEndian( data + 4 );
		cout << "Image dimensions are: " << width << "x" << height << endl;

		bitDepth = *(data + 8);
		colorType = *(data + 9);
		compressionMethod = *(data + 10);
		filterMethod = *(data + 11);
		interlaceMethod = *(data + 12);
	}
	data += length;
	data += 4;
	pointer += length + 12;
	cout << pointer << endl;
}*/


















