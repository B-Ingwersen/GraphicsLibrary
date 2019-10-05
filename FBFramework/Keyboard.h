
char keycodeToASCIITable[] = {
	0, 0x1B,							// escape
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30,	//number keys
	0x2d, 0x3d, 0x08, 0x09,
	0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6f, 0x70, 0x5b, 0x5d,
	0x0d, 0,
	0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x3b, 0x27,
	0x60, 0,
	0x5c, 0x79, 0x78, 0x63, 0x76, 0x62, 0x6e, 0x6d, 0x2c, 0x2e, 0x2f,
	0, 0x2a, 0, 0x20, 0,
};

char keycodeToASCIICapTable[] = {
	0, 0x1b,
	0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a, 0x28, 0x29,
	0x5f, 0x2b, 0x08, 0x09,
	0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4f, 0x50, 0x7b, 0x7d,
	0x0d, 0,
	0x41, 0x53, 0x44, 0x46, 0x47, 0x58, 0x4a, 0x4b, 0x3c, 0x3a, 0x22,
	0x7e, 0,
	0x7c, 0x5a, 0x58, 0x43, 0x56, 0x42, 0x4e,0x4d, 0x3c, 0x3e, 0x3f,
	0, 0x2a, 0, 0x20, 0
};

void processKeys() {
    int fd;
	char path[] = "/dev/input/event3";
    fd = open(path, O_RDONLY);
    struct input_event ev;

    while (!quit)
    {
        ssize_t readSize = read(fd, &ev, sizeof(struct input_event));

		LAST_KEY_ACTION_TYPE = ev.value;
		if(ev.value == 0) {
			//printf("%i released \n ", ev.code);
			int keycode = ev.code;
			if ( keycode == 42 || keycode == 54 ) {
				SHIFT_DOWN = false;
			}
			else if ( keycode == 29 || keycode == 97 ) {
				CTRL_DOWN = false;
			}
			else if ( keycode == 56 || keycode == 100 ) {
				ALT_DOWN = false;
			}

			LAST_KEY_ACTION_PRESS = false;

			if (globalKeyboardHandler != NULL) {
				(*globalKeyboardHandler)( keycode );
				//cout << "called "<< endl;
			}
		}
		else if (ev.value == 1 || ev.value == 2) {
			//printf("%i pressed \n ", ev.code);
			int keycode = ev.code;
			lastKeyDown = keycode;
			if ( keycode == 42 || keycode == 54 ) {
				SHIFT_DOWN = true;
			}
			else if ( keycode == 29 || keycode == 97 ) {
				CTRL_DOWN = true;
			}
			else if ( keycode == 56 || keycode == 100 ) {
				ALT_DOWN = true;
			}
			else if ( CTRL_DOWN && ALT_DOWN && keycode == 14 ) {
				quit = true;
			}

			LAST_KEY_ACTION_PRESS = true;

			if ( !CTRL_DOWN && !ALT_DOWN ) {
				if ( keyboardWindowFocus != NULL ) {
					callWindowEventHandler( keyboardWindowFocus, KEY_PRESS );
				}
			}
			else if (globalKeyboardHandler != NULL) {
				(*globalKeyboardHandler)( keycode );
				//cout << "called "<< endl;
			}

			//cout << keycode << endl;

			lastEvent = clock();
		}
    }
}

char handleKeyDownFRAMEBUFFER( int keycode ) {
	int key;
	if (keycode < 59) {
		if (SHIFT_DOWN) {
			return (int)(keycodeToASCIICapTable[keycode]);
		}
		return (int)(keycodeToASCIITable[keycode]);
	}
	else {
		if (keycode == 111) {
			return 127;
		}
		if (keycode == 106) {
			return -1;
		}
		if (keycode == 105) {
			return -2;
		}
		if (keycode == 102) {
			return -3;
		}
		if (keycode == 107) {
			return -4;
		}
		return 0;
	}
	return 0;
}