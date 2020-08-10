
#ifndef _DRAWING___OFFSCREEN_BUFFERS_H_
#define _DRAWING___OFFSCREEN_BUFFERS_H_

#include "GraphicsLibrary/Definitions.h"

screenData * createOffscreenBuffer( int windowWidth, int windowHeight );

void deleteOffscreenBuffer(screenData * screen);

screenData * duplicateBuffer( screenData * buffer );

screenData * formatOffscreenBuffer( Uint32 * pixels, int width, int height );

void resizeOffscreenBuffer( screenData * screen, int width, int height );

#endif // _DRAWING___OFFSCREEN_BUFFERS_H_