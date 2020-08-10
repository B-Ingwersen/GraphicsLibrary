
#include "GraphicsLibrary/Inputs/Keyboard.h"

char upperCaseNumbers[] = { 60, 95, 62, 63, 41, 33, 64, 35, 36, 37, 94, 38, 42, 40 };

char handleKeyDownSDL( int keycode ) {

	char c;
	if ( keycode >= 97 && keycode <= 122 ) {
		c = keycode;
		if ( SHIFT_DOWN ) { c -= 32; }

		return c;
	}
	else if ( keycode >= 44 && keycode <= 57 ) {
		c = keycode;
		if ( SHIFT_DOWN ) {
			c = upperCaseNumbers[ keycode - 44 ];
		}
		return c;
	}
	else if ( keycode == 61 ) { 
		c = keycode;
		if (SHIFT_DOWN) { c = 43; }
		return c;
	}

	else if (keycode > 32 && keycode < 127) {
		if (SHIFT_DOWN) {
			if (keycode >= 91 && keycode <= 93) {keycode += '}' - ']';}
			else if (keycode == 39) {keycode = 34;}
			else if (keycode == ';') {keycode = ':';}
			else if (keycode == '`') {keycode = '~';}
		}
		return (char)keycode;
	}

	else if ( keycode <= 32 ) {
		if (keycode == 13) {
			return '\n';
		}
		c = keycode; return c;
	}


	if ( keycode == 1073741903 || keycode == 1073741918) {
		c = -1;
		return c;
	}
	else if ( keycode == 1073741904 || keycode == 1073741916) {
		c = -2;
		return c;
	}
	else if ( keycode == 1073741898) {
		c = -3;
		return c;
	}
	else if ( keycode == 1073741901) {
		c = -4;
		return c;
	}
	if ( keycode == 1073741906 || keycode == 1073741920) {
		c = -5;
		return c;
	}
	else if ( keycode == 1073741905 || keycode == 1073741914) {
		c = -6;
		return c;
	}
	else if ( keycode == 127 ) {
		c = keycode;
		return c;
	}

	return 0;
}