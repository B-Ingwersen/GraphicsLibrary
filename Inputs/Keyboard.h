bool SHIFT_DOWN = false;
bool CTRL_DOWN = false;
bool ALT_DOWN = false;

void (*globalKeyboardHandler)( int keycode );

int lastKeyDown;
drawData * keyboardWindowFocus = NULL;

char (*handleKeyDown)(int keycode);

bool LAST_KEY_ACTION_PRESS = false;
int LAST_KEY_ACTION_TYPE;
