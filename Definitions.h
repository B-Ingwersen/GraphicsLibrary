#define FALSE 0
#define TRUE  1

typedef uint32_t Uint32;
struct screenData {
	Uint32 * screen;
	int windowWidth;
	int windowHeight;
};

struct windowSection {
	int X1;
	int Y1;
	int X2;
	int Y2;
	int x;
	int y;
};

struct drawData {
	void (*drawFunction)(drawData self);
	int drawIdentifier;
	int arguments[16];
	windowSection * window;
	int colorInfo[8];
	void * dataPointer;
	void * handlerFunctionsPointer;
	screenData * screen;
};

typedef void (*functionPointer)( drawData * handlingWindow );