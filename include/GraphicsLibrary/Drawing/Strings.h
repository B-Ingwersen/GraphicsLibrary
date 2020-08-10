#ifndef _DRAWING___STRINGS_H_
#define _DRAWING___STRINGS_H_

#include "GraphicsLibrary/Definitions.h"

extern char testText[];

extern char allCharacters[];

extern bool characters[];

extern int charInf_SPACE[];
extern int charInf_EXCLAIM[];
extern int charInf_DBLQUOTE[];
extern int charInf_POUND[];
extern int charInf_DOLSIGN[];
extern int charInf_PERCENT[];
extern int charInf_ADDRESS[];
extern int charInf_SNGLQUOTE[];
extern int charInf_OPENPAREN[];
extern int charInf_CLOSEPAREN[];
extern int charInf_STAR[];
extern int charInf_PLUS[];
extern int charInf_COMMA[];
extern int charInf_MINUS[];
extern int charInf_PERIOD[];
extern int charInf_SLASH[];

extern int charInf_0[];
extern int charInf_1[];
extern int charInf_2[];
extern int charInf_3[];
extern int charInf_4[];
extern int charInf_5[];
extern int charInf_6[];
extern int charInf_7[];
extern int charInf_8[];
extern int charInf_9[];

extern int charInf_COLON[];
extern int charInf_SEMICOLON[];
extern int charInf_LESSTHAN[];
extern int charInf_EQUAL[];
extern int charInf_GREATERTHAN[];
extern int charInf_QUESTION[];
extern int charInf_ATSIGN[];

extern int charInf_A[];
extern int charInf_B[];
extern int charInf_C[];
extern int charInf_D[];
extern int charInf_E[];
extern int charInf_F[];
extern int charInf_G[];
extern int charInf_H[];
extern int charInf_I[];
extern int charInf_J[];
extern int charInf_K[];
extern int charInf_L[];
extern int charInf_M[];
extern int charInf_N[];
extern int charInf_O[];
extern int charInf_P[];
extern int charInf_Q[];
extern int charInf_R[];
extern int charInf_S[];
extern int charInf_T[];
extern int charInf_U[];
extern int charInf_V[];
extern int charInf_W[];
extern int charInf_X[];
extern int charInf_Y[];
extern int charInf_Z[];

extern int charInf_OPENBRACKET[];
extern int charInf_BACKSLASH[];
extern int charInf_CLOSEBRACKET[];
extern int charInf_CARET[];
extern int charInf_UNDER[];
extern int charInf_ACCENT[];

extern int charInf_a[];
extern int charInf_b[];
extern int charInf_c[];
extern int charInf_d[];
extern int charInf_e[];
extern int charInf_f[];
extern int charInf_g[];
extern int charInf_h[];
extern int charInf_i[];
extern int charInf_j[];
//extern int charInf_k[];
extern int charInf_k[];
extern int charInf_l[];
extern int charInf_m[];
extern int charInf_n[];
extern int charInf_o[];
extern int charInf_p[];
extern int charInf_q[];
extern int charInf_r[];
extern int charInf_s[];
extern int charInf_t[];
extern int charInf_u[];
extern int charInf_v[];
extern int charInf_w[];
extern int charInf_x[];
extern int charInf_y[];
extern int charInf_z[];

extern int charInf_OPENBRAC2[];
extern int charInf_SEPERATOR[];
extern int charInf_CLOSEBRAC2[];
extern int charInf_TILDE[];

extern int * betterQualChars[];

extern uint32_t * textPixels;
extern int textWINDOW_WIDTH;

void textRectangle( int x, int y, int width, int height, int color, windowSection WINDOW);

void drawShearLine( int x, int y, int width, int height, int size, int color, windowSection WINDOW );

void betterQualChar( int * charInfo, int color, int x, int y, int size, windowSection WINDOW );

void betterQualString( drawData drawInformation );

void drawCharacter(char character, int x ,int y ,int size ,int color,  windowSection WINDOW );

void drawString(drawData drawInformation);

#endif // _DRAWING___STRINGS_H_
