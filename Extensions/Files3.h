#include <fstream>
#include <iostream>
#include <stdio.h>
#include "extensions/SDLDataOperations.h"

#include "extensions/filesHeaders/SDLJpeg.h"
#include "extensions/filesHeaders/SDLPNG.h"
#include "extensions/filesHeaders/SDLMP4.h"

struct fileData {
	unsigned char * buffer;
	int fileSize;
};

fileData * openFileToBuffer( char * fileName ) {
	FILE * file;
	file = fopen( fileName, "r" );

	if ( file != NULL ) {

		fseek( file, 0L, SEEK_END);
		int fileSize = ftell( file );

		unsigned char * buffer = new unsigned char[ fileSize ];
		fileData * fileInfo = new fileData;
		fileInfo -> buffer = buffer;
		fileInfo -> fileSize = fileSize;

		rewind( file );
		int freadReturn = fread( buffer, fileSize, 1, file );
		fclose( file );

		return fileInfo;

	}
	return NULL;
}

unsigned char PNGHeader[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
unsigned char JPEGMagicNumber[] = { 0xFF, 0xD8 };

screenData * openPicture( char * fileName ) {

	fileData * file = openFileToBuffer( fileName );

	if ( file != NULL ) {
		unsigned char * dataBuffer = file -> buffer;
		int fileSize = file -> fileSize;

		if ( compareData( dataBuffer, PNGHeader, 8 ) ) {
			screenData * PNGPicture = renderPNGImage( dataBuffer, fileSize );
			delete dataBuffer;
			delete file;
			return PNGPicture;
		}

		else if ( compareData( dataBuffer, JPEGMagicNumber, 2) ) {
			screenData * JPEGPicture = renderJPEGImage( dataBuffer, fileSize );
			delete dataBuffer;
			delete file;
			return JPEGPicture;
		}
	}
	delete file;
	return NULL;
}

void openVideo( char * fileName ) {
	fileData * file = openFileToBuffer( fileName );

	if ( file != NULL ) {
		cout << "File size:\t" << file -> fileSize << endl;
		unsigned char * dataBuffer = file -> buffer;
		int fileSize = file -> fileSize;

		openMP4Video( dataBuffer, fileSize );

	}
	delete file;
	//return NULL;
}



















