
#ifndef _INPUTS_KEYBOARD___H_
#define _INPUTS_KEYBOARD___H_

#include "GraphicsLibrary/Definitions.h"

extern void (*globalKeyboardHandler)( int keycode );
extern char (*handleKeyDown)(int keycode);

extern bool SHIFT_DOWN;
extern bool CTRL_DOWN;
extern bool ALT_DOWN;

extern int lastKeyDown;
extern int lastKeyUp;
extern drawData * keyboardWindowFocus;

extern bool LAST_KEY_ACTION_PRESS;
extern int LAST_KEY_ACTION_TYPE;

#endif // _INPUTS_KEYBOARD___H_
