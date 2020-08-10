
#include "GraphicsLibrary/Core.h"

struct fileData {
	unsigned char * buffer;
	int fileSize;
};

fileData * openFileToBuffer( char * fileName );

void writeFileToBuffer( char * fileName, unsigned char * buffer, int length );

extern unsigned char PNGHeader[];
extern unsigned char JPEGMagicNumber[];

screenData * openPicture( char * fileName );

void openVideo( char * fileName );



















