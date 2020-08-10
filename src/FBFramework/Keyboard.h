
#ifndef _FB_FRAMEWORK___KEYBOARD_H_
#define _FB_FRAMEWORK___KEYBOARD_H_

extern char keycodeToASCIITable[];
extern char keycodeToASCIICapTable[];

void processKeys();
char handleKeyDownFRAMEBUFFER( int keycode );

#endif // _FB_FRAMEWORK___KEYBOARD_H_