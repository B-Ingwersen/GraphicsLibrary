#include "Framebuffer.h"
#include "Mouse.h"
#include "Keyboard.h"

void FBFramework_prepProcessEvents() {
    // Nothing required
}
void FBFramework_displayProcessedEvents() {
    memcpy( FRAME_BUFFER_MASTER, onScreen -> screen, FB_SCREENSIZE );
    drawCursor();
}

void FBFramework_EventLoop() {
    handleKeyDown = handleKeyDownFRAMEBUFFER;

    pthread_t * thread = new pthread_t;

    pthread_create( thread, NULL, processMouse, NULL);
    lastEvent = 0;
    processKeys();

    memset( FRAME_BUFFER_MASTER, 0, FB_SCREENSIZE );
    disconnectFrameBuffer();
    int returnValue = system("stty echo");
}

bool FBFramework_Initialize() {

    getFrameBuffer();

    int returnValue = system("stty -echo");
    returnValue = system("echo -n -e '\e[?17;14;224c'");

    onScreen = createOffscreenBuffer(WINDOW_WIDTH_PARENT, WINDOW_HEIGHT_PARENT);

    FRAME_BUFFER_SCREEN.screen = (Uint32*)(FRAME_BUFFER_MASTER);
    FRAME_BUFFER_SCREEN.windowWidth = WINDOW_WIDTH_PARENT;
    FRAME_BUFFER_SCREEN.windowHeight = WINDOW_HEIGHT_PARENT;

    mouseX = WINDOW_WIDTH_PARENT / 2;
    mouseY = WINDOW_HEIGHT_PARENT / 2;

    prepProcessEvents = FBFramework_prepProcessEvents;
    displayProcessedEvents = FBFramework_displayProcessedEvents;
    eventLoop = FBFramework_EventLoop;

    return true;
}