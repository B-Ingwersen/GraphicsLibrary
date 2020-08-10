#ifndef _RENDER_INFASTRUCTURE___SIZE_CHANGE_
#define _RENDER_INFASTRUCTURE___SIZE_CHANGE_

#include "GraphicsLibrary/Definitions.h"

extern void (*sizeChangeFunction)();

void setResizeFunc( drawData * window, void function(drawData * window) );

extern pthread_spinlock_t sizeChangeSpinLock;

void reconfigureWindowDimensions( drawData * window );

void reconfigWindowDimsTemplate( drawData * window, void manipFunc( drawData * window, drawData * parentWindow) );

void resizeHManip( drawData * window, drawData * parentWindow );
void resizeH( drawData * window );

void resizeWManip( drawData * window, drawData * parentWindow );
void resizeW( drawData * window );

void resizeHWManip( drawData * window, drawData * parentWindow );
void resizeHW( drawData * window );

void resizeBRManip( drawData * window, drawData * parentWindow );
void resizeBR( drawData * window );

void resizeBWManip( drawData * window, drawData * parentWindow );
void resizeBW( drawData * window );

void resizeCxManip( drawData * window, drawData * parentWindow );
void resizeCx( drawData * window );

void resizeBCxManip( drawData * window, drawData * parentWindow );
void resizeBCx( drawData * window );

void resizeCxyManip( drawData * window, drawData * parentWindow );
void resizeCxy( drawData * window );

void resizeRManip( drawData * window, drawData * parentWindow );
void resizeR( drawData * window );

#endif // _RENDER_INFASTRUCTURE___SIZE_CHANGE_