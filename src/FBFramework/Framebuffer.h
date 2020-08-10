
#ifndef _FB_FRAMEWORK___FRAMEBUFFER_H_
#define _FB_FRAMEWORK___FRAMEBUFFER_H_

#include "GraphicsLibrary/Definitions.h"

extern char * FRAME_BUFFER_MASTER;
extern int FBFD_MASTER;
extern int FB_SCREENSIZE;
extern screenData FRAME_BUFFER_SCREEN;

bool getFrameBuffer();

void disconnectFrameBuffer();

#endif // _FB_FRAMEWORK___FRAMEBUFFER_H_