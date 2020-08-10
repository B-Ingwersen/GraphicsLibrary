
#include "GraphicsLibrary/Core.h"
#include "GraphicsLibrary/Extensions/Files.h"

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef unsigned short u2be;
typedef unsigned int u4be;

typedef int i32;
typedef short i16;
typedef char i8;
typedef short s2be;
typedef int s4be;

extern char defaultFontSans[];
extern char defaultFontSerif[];
extern char defaultFontMono[];

struct TtfOffsetTable {
    u32 sfnt_version;
    u2be num_tables;
    u2be search_range;
    u2be entry_selector;
    u2be range_shift;
} __attribute__((packed));

struct TtfDirTableEntry {
    char tag[4];
    u4be checksum;
    u4be offset;
    u4be length;
} __attribute__((packed));

struct ttfRoot {
    TtfOffsetTable OffsetTable;
} __attribute__((packed));

struct ttfGlyf {
    s2be number_of_contours;
    s2be x_min;
    s2be y_min;
    s2be x_max;
    s2be y_max;
} __attribute__((packed));

struct ttfHead {
    u32 version;
    u32 font_revision;
    u4be checksum_adjustment;
    u32 magic_number;
    u2be flags;
    u2be units_per_em;

    u4be created_0;
    u4be created_1;

    u4be modified_0;
    u4be modified_1;
    s2be x_min;
    s2be y_min;
    s2be x_max;
    s2be y_max;
    u2be mac_style;
    u2be lowest_rec_ppem;
    s2be font_direction_hint;
    s2be index_to_loc_format;
    s2be glyph_data_format;

} __attribute__((packed));

struct ttfHhea {
    u32 version;
    s2be ascender;
    s2be descender;
    s2be line_gap;
    u2be advance_width_max;
    s2be min_left_wide_bearing;
    s2be min_right_side_bearing;
    s2be x_max_extend;
    s2be caret_slope_rise;
    s2be caret_slope_run;
    u8 reserved[10];
    s2be metric_data_format;
    u2be number_of_hmetrics;
} __attribute__((packed));

struct ttfHMetric {
    u2be advanceWidth;
    s2be leftSideBearing;
} __attribute__((packed));

struct ttfCmapHeader {
    u2be version;
    u2be numTables;
} __attribute__((packed));

struct ttfCmapSubtableHeader {
    u2be platformID;
    u2be encodingID;
    u4be subtableOffset;
} __attribute__((packed));

struct ttfKernSubtableFormat0KerningPair {
    u2be left;
    u2be right;
    s2be value;
} __attribute__((packed));

struct ttfTrackTable {
    u32 version;
    u2be format;
    u2be horizOffset;
    u2be vertOffset;
    u2be reserved;
} __attribute__((packed));

struct ttfTrackData {
    u2be nTracks;
    u2be nSizes;
    u4be sizeTableOffset;
} __attribute__((packed));

struct ttfTrackDataEntry {
    i32 track;
    u2be nameIndex;
    u2be offset;
} __attribute__((packed));

struct glyphInfo {
    u8 * flags;
    int nPoints;
    int * points;

    int nContours;
    int * endpoints;

    int w;
    int h;
    int x1;
    int y1;
    int x2;
    int y2;
    int advanceWidth;

    int X1;
    int Y1;
    int X2;
    int Y2;

    int glyphIndex;
};

struct fontCharacterCache {
	screenData * screen;
	float fontWidth;
	float fontHeight;
	float xBufferOffset;
	float yBufferOffset;
    float advanceWidth;
	int glyphIndex;

    float totalWidth;
    float totalHeight;
};

struct trueTypeFontRenderer;
struct trueTypeFontDescriptor {
    trueTypeFontDescriptor * nextDescriptor;
    trueTypeFontDescriptor * previousDescriptor;

    trueTypeFontRenderer * rootRenderer;

    char * fontFileName;
    fileData * fontFileRaw;
    int referenceCounter;

    glyphInfo * glyphInfoCache[128];
    unsigned char * kernTablePointer;
};

struct trueTypeFontRenderer {
    trueTypeFontRenderer * nextRenderer;
    trueTypeFontRenderer * previousRenderer;

    trueTypeFontDescriptor * descriptor;
    float fontSize;
    int aliasScale;
    float scale;

    bool monoSpace;
    float monoSpaceWidth;
    float tabWidth;

    fontCharacterCache * cache[128];
    int referenceCounter;
};

struct trueTypeFontManager {
    trueTypeFontDescriptor * descriptor;
};

u32 u4beToLE(u4be x);

u16 u2beToLE(u2be x);

i32 s4beToLE(s4be x);

i16 s2beToLE(s2be x);

void * getTtfTable(ttfRoot * t, const char name[4]);

void deleteGlyphInfo(glyphInfo * info);

int getCharacterGlyphIndex(ttfRoot * t, int c);

int getGlyphOffset(ttfRoot * t, int offset);

void getGlyphAdvanceWidth(ttfRoot * t, int glyphIndex, glyphInfo * info);

glyphInfo * getGlyphInfo(ttfRoot * t, int c);

void getTrackingTable(ttfRoot * t);

void getHTMXTable(ttfRoot * t);

screenData * downsizeCharacterBuffer(unsigned char * pixels, int w, int h, int downSize);

void drawTriangleFixBezier(float x1i, float y1i, float x2i, float y2i, float x3i, float y3i, unsigned char * pixels, int WINDOW_WIDTH, int WINDOW_HEIGHT);

void drawTriangleForPoly(float x1i, float y1i, float x2i, float y2i, float x3i, float y3i, unsigned char * pixels, int WINDOW_WIDTH, int WINDOW_HEIGHT);

void copyBufferRGBShade(screenData * sourceBuffer, screenData * targetBuffer, int xSource, int ySource, int xTarget, int yTarget, int width, int height);

void deleteFontCharacterCache(fontCharacterCache * cache);

fontCharacterCache * drawTrueTypeCharacter(int c, trueTypeFontRenderer * renderer);

trueTypeFontRenderer * openTrueTypeRenderer(trueTypeFontDescriptor * fontDescriptor, float fontSize);

trueTypeFontDescriptor * openTrueTypeFont(trueTypeFontManager * fontManager, char * fontFileName);

void deleteTrueTypeRenderer(trueTypeFontDescriptor * descriptor, trueTypeFontRenderer * renderer);

void deleteTrueTypeDescriptor(trueTypeFontManager * manager, trueTypeFontDescriptor * descriptor);

void closeTrueTypeDescriptor(trueTypeFontManager * manager, trueTypeFontDescriptor * descriptor);

void closeTrueTypeRenderer(trueTypeFontDescriptor * descriptor, trueTypeFontRenderer * renderer);

float getKerningPair(trueTypeFontRenderer * renderer, i16 l, i16 r);

void drawTrueTypeText(trueTypeFontRenderer * renderer, char * text, int x, int y, float maxWidth, bool draw);