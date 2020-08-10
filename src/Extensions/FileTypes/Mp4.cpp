
#include "GraphicsLibrary/Extensions/DataOperations.h"
#include <iostream>

void transformMatrix( int p, int q, int * matrix, int * store ) {
	int m = matrix[1] * p + matrix[4] * q + matrix[7];
	int n = matrix[2] * p + matrix[5] * q + matrix[8];
	int z = matrix[3] * p + matrix[8] * q + matrix[9];
	store[0] = m/z;
	store[1] = n/z;
}

unsigned char MP4Tag_ftyp[] = "ftyp";
unsigned char MP4Tag_mdat[] = "mdat";
unsigned char MP4Tag_moov[] = "moov";

unsigned char mmp4Subtype[] = "mmp4";

unsigned char moov_trak[] = "trak";
unsigned char moov_mvhd[] = "mvhd";
unsigned char moov_udta[] = "udta";

unsigned char trak_tkhd[] = "tkhd";
unsigned char trak_edts[] = "edts";
unsigned char trak_mdia[] = "mdia";

unsigned char mdia_mdhd[] = "mdhd";
unsigned char mdia_minf[] = "minf";
unsigned char mdia_hdlr[] = "hdlr";

unsigned char minf_gmhd[] = "gmhd";
unsigned char minf_smhd[] = "smhd";
unsigned char minf_stbl[] = "stbl";
unsigned char minf_vmhd[] = "vmhd";
unsigned char minf_hdlr[] = "hdlr";

unsigned char stbl_co64[] = "co64";
unsigned char stbl_ctts[] = "ctts";
unsigned char stbl_stco[] = "stco";
unsigned char stbl_stsc[] = "stsc";
unsigned char stbl_stsd[] = "stsd";
unsigned char stbl_stss[] = "stss";
unsigned char stbl_stsz[] = "stsz";
unsigned char stbl_stts[] = "stts";

void tp( int t ) {
	int i;
	for ( i = 0; i < t; i++ ) {
		std::cout << "\t";
	}
}

void processStsc( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stsc block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 8; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int n = getDwordBE( data, 12 );
	tp(t); std::cout << "\tn:\t" << n << std::endl;
	for ( i = 0; i < n; i++ ) {
		tp(t); std::cout << "\t" << getDwordBE( data, 16 + 12 * i );
		std::cout << "\t" << getDwordBE( data, 20 + 12 * i );
		std::cout << "\t" << getDwordBE( data, 24 + 12 * i ) << std::endl;
	}

	
}

void processStco( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stco block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 8; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int n = getDwordBE( data, 12 );
	tp(t); std::cout << "\tn:\t" << n << std::endl;
	for ( i = 0; i < n; i++ ) {
		tp(t); std::cout << "\t" << getDwordBE( data, 16 + 4 * i ) << std::endl;
	}
}

void processStsz( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stsz block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 8; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int size = getDwordBE( data, 12 );
	if ( size != 0 ) {
		tp(t); std::cout << "\tUniformSize:\t" << size << std::endl;
		int n = getDwordBE( data, 16 );
		tp(t); std::cout << "\tNumOfSamp:\t" << n << std::endl;
	}
	else {
		int n = getDwordBE( data, 16 );
		tp(t); std::cout << "\tn:\t" << n << std::endl;

		tp(t);
		for ( i = 0; i < n; i++ ) {
			//std::cout << "\t" << getDwordBE( data, 20 + 4 * i );
		}
		tp(t); std::cout << std::endl;
	}
}

void processStss( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stss block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 8; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int n = getDwordBE( data, 12 );
	tp(t); std::cout << "\tn:\t" << n << std::endl;
	for ( i = 0; i < n; i++ ) {
		tp(t); std::cout << "\t" << getDwordBE( data, 16 + 4 * i ) << std::endl;
	}
}

void processStts( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stts block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 8; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int n = getDwordBE( data, 12 );

	int numOfSamples;
	for ( i = 0; i < n; i++ ) {
		numOfSamples += getDwordBE( data, 16 + 8 * i );
	}

	//std::cout << "NSAMPLES\t" << n << std::endl;

	/*int * timeSampleData = new int[ 2 * numOfSamples ];
	tp(t); std::cout << "\tn:\t" << numOfSamples << std::endl;

	int sampNum = 0;
	int sampTime = 0;
	for ( i = 0; i < n; i++ ) {
		//tp(t); std::cout << "\t" << getDwordBE( data, 16 + 8 * i );
		//tp(t); std::cout << "\t" << getDwordBE( data, 20 + 8 * i ) << std::endl;

		int sampleCount = getDwordBE( data, 16 + 8 * i );
		int sampleDuration = getDwordBE( data, 20 + 8 * i );
		int j;
		for ( j = 0; j < sampleCount; j++ ) {
			timeSampleData[ 2 * sampNum ] = sampTime;
			timeSampleData[ 2 * sampNum + 1 ] = sampTime + sampleDuration;

			tp(t); std::cout << "\t" << sampNum << "\t" << timeSampleData[ 2 * sampNum ] << "\t" << timeSampleData[ 2 * sampNum + 1 ] << std::endl;

			sampTime += sampleDuration;
			sampNum++;
		}

	}
	tp(t); std::cout << std::endl;*/
}

void processStsd( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stsd block" << std::endl;

	int version = data[8];
	tp(t); std::cout << "\tver:\t" << version << std::endl;

	int i;
	for ( i = 9; i < 12; i++ ) {
		tp(t); std::cout << "\tflag:\t" << (int)(data[i]) << std::endl;
	}

	int n = getDwordBE( data, 12 );
	tp(t); std::cout << "\tn:\t" << n << std::endl;
	int index = 16;
	for ( i = 0; i < n; i++ ) {
		int subLength = getDwordBE( data, index );
		tp(t); std::cout << "\tSmplDescriptor:\t" << i << std::endl;

		int dataFormat = getDwordBE( data, index + 4 );
		int dataRefIndex = data[ index + 10 ] * 256 + data[ index + 11 ];

		tp(t); std::cout << "\t\tLength:  \t" << subLength << std::endl;
		tp(t); std::cout << "\t\tdataFormat:\t" << dataFormat << std::endl;
		tp(t); std::cout << "\t\tdataRefInd:\t" << dataRefIndex << std::endl;

		index += subLength;
	}
}

void processStbl( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "stbl block" << std::endl;

	int index = 8;
	while ( index < length ) {
		int subLength = getDwordBE( data, index );
		//tp(t); std::cout << "\tStblSublock @\t" << index << std::endl;
		//tp(t); std::cout << "\t Size\t\t" << subLength << std::endl;

		if ( compareData( data + index + 4, stbl_co64, 4 ) ) {
			tp(t); std::cout << "co64" << std::endl;
		}
		if ( compareData( data + index + 4, stbl_ctts, 4 ) ) {
			tp(t); std::cout << "ctts" << std::endl;
		}
		if ( compareData( data + index + 4, stbl_stco, 4 ) ) {
			processStco( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, stbl_stsc, 4 ) ) {
			processStsc( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, stbl_stsd, 4 ) ) {
			processStsd( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, stbl_stss, 4 ) ) {
			processStsc( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, stbl_stsz, 4 ) ) {
			processStsz( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, stbl_stts, 4 ) ) {
			processStts( data + index, subLength, t + 1 );
		}


		index += subLength;
	}
}

struct videoMediaHeader {
	int version;
	int flags[3];
	int graphicsMode;

	int opColorRed;
	int opColorGreen;
	int opColorBlue;
};

void processVmhd( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "vmhd block" << std::endl;

	videoMediaHeader * vmhd = new videoMediaHeader;
	vmhd -> version = data[8];
	vmhd -> flags[0] = data[9];
	vmhd -> flags[1] = data[10];
	vmhd -> flags[2] = data[11];

	vmhd -> graphicsMode = data[12] * 256 + data[13];
	vmhd -> opColorRed = data[14] * 256 + data[15];
	vmhd -> opColorGreen = data[16] * 256 + data[17];
	vmhd -> opColorBlue = data[18] * 256 + data[19];

	tp(t); std::cout << "\tgraphicsMode:\t" << vmhd -> graphicsMode << std::endl;
	tp(t); std::cout << "\topColorRed:\t" << vmhd -> opColorRed << std::endl;
	tp(t); std::cout << "\topColorGreen:\t" << vmhd -> opColorGreen << std::endl;
	tp(t); std::cout << "\topColorBlue:\t" << vmhd -> opColorBlue << std::endl;

}

struct handlerReference {
	int version;
	int flags[3];
	int compType;
	int compSubType;
	int compManufac;
	int compFlags;
	int compFlagMask;
};

void processHdlr( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "hdlr block" << std::endl;

	handlerReference * hdlr = new handlerReference;
	hdlr -> version = data[8];
	hdlr -> flags[0] = data[9];
	hdlr -> flags[1] = data[10];
	hdlr -> flags[2] = data[11];

	hdlr -> compType = getDwordBE( data, 12 );
	hdlr -> compSubType = getDwordBE( data, 16 );
	hdlr -> compManufac = getDwordBE( data, 20 );
	hdlr -> compFlags = getDwordBE( data, 24 );
	hdlr -> compFlagMask = getDwordBE( data, 28 );

	tp(t); std::cout << "\tcompType:\t" << hdlr -> compType << std::endl;
	tp(t); std::cout << "\tcompSubType:\t" << hdlr -> compSubType << std::endl;
	tp(t); std::cout << "\tcompManufac:\t" << hdlr -> compManufac << std::endl;
	tp(t); std::cout << "\tcompFlags:\t" << hdlr -> compFlags << std::endl;
	tp(t); std::cout << "\tcompFlagMask:\t" << hdlr -> compFlagMask << std::endl;
}

struct soundMediaHeader {
	int version;
	int flags[3];
	int balance;
};

void processSmhd( unsigned char * data, int length, int t ) {
	soundMediaHeader * smhd = new soundMediaHeader;

	smhd -> version = data[8];
	smhd -> flags[0] = data[9];
	smhd -> flags[1] = data[10];
	smhd -> flags[2] = data[11];

	smhd -> balance = data[12] * 256 + data[13];

	tp(t); std::cout << "smhd block" << std::endl;
	tp(t); std::cout << "\tversion:\t" << smhd -> version << std::endl;
	tp(t); std::cout << "\tbalance:\t" << smhd -> balance << std::endl;
}

void processMinf( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "minf block" << std::endl;

	int index = 8;
	while ( index < length ) {
		int subLength = getDwordBE( data, index );
		//tp(t); std::cout << "\tMinfSublock @\t" << index << std::endl;
		//tp(t); std::cout << "\t Size\t\t" << subLength << std::endl;

		if ( compareData( data + index + 4, minf_gmhd, 4 ) ) {
			tp(t); std::cout << "gmhd" << std::endl;
		}
		if ( compareData( data + index + 4, minf_smhd, 4 ) ) {
			processSmhd( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, minf_stbl, 4 ) ) {
			processStbl( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, minf_vmhd, 4 ) ) {
			processVmhd( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, minf_hdlr, 4 ) ) {
			processHdlr( data + index, subLength, t + 1 );
		}

		index += subLength;
	}
}

struct mediaHeader {
	int version;
	int flags[3];
	unsigned int creationTime;
	unsigned int modificationTime;
	int timeScale;
	int duration;
	int language;
	int quality;
};

void processMdhd( unsigned char * data, int length, int t ) {
	mediaHeader * mdhd = new mediaHeader;

	mdhd -> version = data[8];
	mdhd -> flags[0] = data[9];
	mdhd -> flags[1] = data[10];
	mdhd -> flags[2] = data[11];

	mdhd -> creationTime = getDwordBE( data, 12 );
	mdhd -> modificationTime = getDwordBE( data, 16 );
	mdhd -> timeScale = getDwordBE( data, 20 );
	mdhd -> duration = getDwordBE( data, 24 );
	mdhd -> language = data[28] * 256 + data[29];
	mdhd -> quality = data[30] * 256 + data[31];

	tp(t); std::cout << "media header" << std::endl;
	tp(t); std::cout << "\tversion:\t" << mdhd -> version << std::endl;
	tp(t); std::cout << "\tcreationTime:\t" << mdhd -> creationTime << std::endl;
	tp(t); std::cout << "\tmodTime:\t" << mdhd -> modificationTime << std::endl;
	tp(t); std::cout << "\ttimeScale:\t" << mdhd -> timeScale << std::endl;
	tp(t); std::cout << "\tduration:\t" << mdhd -> duration << std::endl;
	tp(t); std::cout << "\tlanguage:\t" << mdhd -> language << std::endl;
	tp(t); std::cout << "\tquality:\t" << mdhd -> quality << std::endl;
}

void processMdia( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "mdia block" << std::endl;

	int index = 8;
	while ( index < length ) {
		int subLength = getDwordBE( data, index );
		//tp(t); std::cout << "\tMdiaSublock @\t" << index << std::endl;
		//tp(t); std::cout << "\t Size\t" << subLength << std::endl;

		if ( compareData( data + index + 4, mdia_mdhd, 4 ) ) {
			processMdhd( data + index, subLength, t + 1 );
		}

		if ( compareData( data + index + 4, mdia_minf, 4 ) ) {
			processMinf( data + index, subLength, t + 1 );
		}

		if ( compareData( data + index + 4, mdia_hdlr, 4 ) ) {
			processHdlr( data + index, subLength, t + 1 );
		}

	index += subLength;
	}
}

struct trackHeader {
	int version;
	int flags[3];
	unsigned int creationTime;
	unsigned int modificationTime;
	int trackID;
	int duration;
	int layer;
	int alternateGroup;
	int volume;
	int matrix[9];
	int trackWidth;
	int trackHeight;
};

void processTkhd( unsigned char * data, int subLength, int t ) {
	trackHeader * tkhd = new trackHeader;

	tkhd -> version = data[8];
	tkhd -> flags[0] = data[9];
	tkhd -> flags[1] = data[10];
	tkhd -> flags[2] = data[11];
	tkhd -> creationTime = getDwordBE( data, 12 );
	tkhd -> modificationTime = getDwordBE( data, 16 );
	tkhd -> trackID = getDwordBE( data, 20 );
	tkhd -> duration = getDwordBE( data, 28 );
	tkhd -> layer = data[40] * 256 + data[41];
	tkhd -> alternateGroup = data[42] * 256 + data[43];
	tkhd -> volume = data[44] * 256 + data[45];

	tkhd -> matrix[0] = getDwordBE( data, 48 );
	tkhd -> matrix[1] = getDwordBE( data, 52 );
	tkhd -> matrix[2] = getDwordBE( data, 56 );
	tkhd -> matrix[3] = getDwordBE( data, 60 );
	tkhd -> matrix[4] = getDwordBE( data, 64 );
	tkhd -> matrix[5] = getDwordBE( data, 68 );
	tkhd -> matrix[6] = getDwordBE( data, 72 );
	tkhd -> matrix[7] = getDwordBE( data, 76 );
	tkhd -> matrix[8] = getDwordBE( data, 80 );

	tkhd -> trackWidth = getDwordBE( data, 84 ) >> 16;
	tkhd -> trackHeight = getDwordBE( data, 88 ) >> 16;

	tp(t); std::cout << "track header" << std::endl;
	tp(t); std::cout << "\tversion:\t" << tkhd -> version << std::endl;
	tp(t); std::cout << "\tcreationTime:\t" << tkhd -> creationTime << std::endl;
	tp(t); std::cout << "\tmodTime:\t" << tkhd -> modificationTime << std::endl;
	tp(t); std::cout << "\ttrackID:\t" << tkhd -> trackID << std::endl;
	tp(t); std::cout << "\tduration:\t" << tkhd -> duration << std::endl;
	tp(t); std::cout << "\tlayer:  \t" << tkhd -> layer << std::endl;
	tp(t); std::cout << "\taltGroup:\t" << tkhd -> alternateGroup << std::endl;
	tp(t); std::cout << "\tvolume: \t" << tkhd -> volume << std::endl;
	tp(t); std::cout << "\ttrackWidth:\t" << tkhd -> trackWidth << std::endl;
	tp(t); std::cout << "\ttrackHeight:\t" << tkhd -> trackHeight << std::endl;
}

void processTrak( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "trak block" << std::endl;
	int index = 8;
	while ( index < length ) {
		int subLength = getDwordBE( data, index );
		//tp(t); std::cout << "\tTrakSublock @\t" << index << std::endl;
		//tp(t); std::cout << "\t Size\t" << subLength << std::endl;

		if ( compareData( data + index + 4, trak_tkhd, 4 ) ) {
			processTkhd( data + index, subLength, t + 1 );
		}

		if ( compareData( data + index + 4, trak_edts, 4 ) ) {
			tp(t); std::cout << "\t edtsSublock" << std::endl;
		}
		if ( compareData( data + index + 4, trak_mdia, 4 ) ) {
			processMdia( data + index, subLength, t + 1 );
		}


		index += subLength;
	}
}

struct movieHeader {
	int version;
	int flags[3];
	unsigned int creationTime;
	unsigned int modTime;
	int timeScale;
	int duration;
	int preferredRate;
	int preferredVolume;
	int matrix[9];
	int previewTime;
	int previewDuration;
	int posterTime;
	int selectionTime;
	int selectionDuration;
	int currentTime;
	int nextTrackID;
};

void processMvhd( unsigned char * data, int subLength, int t ) {
	movieHeader * mvhd = new movieHeader;

	mvhd -> version = data[8];
	mvhd -> flags[0] = data[9];
	mvhd -> flags[1] = data[10];
	mvhd -> flags[2] = data[11];
	mvhd -> creationTime = getDwordBE( data, 12 );
	mvhd -> modTime = getDwordBE( data, 16 );
	mvhd -> timeScale = getDwordBE( data, 20 );
	mvhd -> duration = getDwordBE( data, 24 );
	mvhd -> preferredRate = getDwordBE( data, 28 );
	mvhd -> preferredVolume = data[33] + 256 * data[32];

	mvhd -> matrix[0] = getDwordBE( data, 44 );
	mvhd -> matrix[1] = getDwordBE( data, 48 );
	mvhd -> matrix[2] = getDwordBE( data, 52 );
	mvhd -> matrix[3] = getDwordBE( data, 56 );
	mvhd -> matrix[4] = getDwordBE( data, 60 );
	mvhd -> matrix[5] = getDwordBE( data, 64 );
	mvhd -> matrix[6] = getDwordBE( data, 68 );
	mvhd -> matrix[7] = getDwordBE( data, 72 );
	mvhd -> matrix[8] = getDwordBE( data, 76 );

	mvhd -> previewTime = getDwordBE( data, 80 );
	mvhd -> previewDuration = getDwordBE( data, 84 );
	mvhd -> posterTime = getDwordBE( data, 88 );
	mvhd -> selectionTime = getDwordBE( data, 92 );
	mvhd -> selectionDuration = getDwordBE( data, 96 );
	mvhd -> currentTime = getDwordBE( data, 100 );
	mvhd -> nextTrackID = getDwordBE( data, 104 );

	tp(t); std::cout << "Movie header block" << std::endl;
	tp(t); std::cout << "\tversion:\t" << mvhd -> version << std::endl;
	tp(t); std::cout << "\tcreationTime:\t" << mvhd -> creationTime << std::endl;
	tp(t); std::cout << "\tmodTime:\t" << mvhd -> modTime << std::endl;
	tp(t); std::cout << "\ttimeScale:\t" << mvhd -> timeScale << std::endl;
	tp(t); std::cout << "\tduration:\t" << mvhd -> duration << std::endl;
	tp(t); std::cout << "\tprefRate:\t" << mvhd -> preferredRate << std::endl;
	tp(t); std::cout << "\tprefVolume:\t" << mvhd -> preferredVolume << std::endl;
	tp(t); std::cout << "\tprevTime:\t" << mvhd -> previewTime << std::endl;
	tp(t); std::cout << "\tprevDuration:\t" << mvhd -> previewDuration << std::endl;
	tp(t); std::cout << "\tposterTime:\t" << mvhd -> posterTime << std::endl;
	tp(t); std::cout << "\tselecTime:\t" << mvhd -> selectionTime << std::endl;
	tp(t); std::cout << "\tselecDuration:\t" << mvhd -> selectionDuration << std::endl;
	tp(t); std::cout << "\tcurrentTime:\t" << mvhd -> currentTime << std::endl;
	tp(t); std::cout << "\tnextTrackID:\t" << mvhd -> nextTrackID << std::endl;
}

void processUdta( unsigned char * data, int length, int t ) {
	tp(t); std::cout << "udta block" << std::endl;
}

void processMoov( unsigned char * data, int length, int t ) {
	tp(t); std::cout << " moov block" << std::endl;

	int index = 8;
	while ( index < length ) {
		int subLength = getDwordBE( data, index );
		//tp(t); std::cout << "\tMoovSublock @\t" << index << std::endl;
		//tp(t); std::cout << "\t Size\t" << subLength << std::endl;

		if ( compareData( data + index + 4, moov_trak, 4 ) ) {
			//tp(t); std::cout << "\t trak block" << std::endl;
			processTrak( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, moov_mvhd, 4 ) ) {
			//tp(t); std::cout << "\t mvhd block" << std::endl;
			processMvhd( data + index, subLength, t + 1 );
		}
		if ( compareData( data + index + 4, moov_udta, 4 ) ) {
			//tp(t); std::cout << "\t mvhd block" << std::endl;
			processUdta( data + index, subLength, t + 1 );
		}

		index += subLength;
	}
}

void openMP4Video( unsigned char * dataBuffer, int fileSize ) {
	int index = 0;
	int t = 0;
	while ( index < fileSize ) {
		int length = getDwordBE( dataBuffer, index );
		tp(t); std::cout << "Segment @ \t" << index << std::endl;
		tp(t); std::cout << " Size\t" << length << std::endl;

		if ( compareData( dataBuffer + index + 4, MP4Tag_ftyp, 4 ) ) {
			tp(t); std::cout << " ftyp block" << std::endl;
			//if ( !compareData( dataBuffer + index + 4, mmp4Subtype, 4 ) ) { return; }
			tp(t); std::cout << " mmp4 subtype" << std::endl;
		}

		if ( compareData( dataBuffer + index + 4, MP4Tag_mdat, 4 ) ) {
			tp(t); std::cout << " mdat block" << std::endl;
		}

		if ( compareData( dataBuffer + index + 4, MP4Tag_moov, 4 ) ) {
			processMoov( dataBuffer + index, length, t + 1 );
			/*std::cout << " moov block" << std::endl;

			int subIndex = 8;
			while ( subIndex < length ) {
				int subLength = getDwordBE( dataBuffer, index + subIndex );
				std::cout << "\tMoovSublock @\t" << subIndex << std::endl;
				std::cout << "\t Size\t" << subLength << std::endl;

		if ( compareData( dataBuffer + index + subIndex + 4, moov_trak, 4 ) ) {
			std::cout << "\t trak block" << std::endl;
			processTrak( dataBuffer + index + subIndex, subLength );
		}

				subIndex += subLength;
			}*/

		}

		std::cout << std::endl;
		index += length;
	}
}
