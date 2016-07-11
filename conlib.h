#ifndef CONLIB
#define CONLIB

#include <stdio.h>


// Color defs
#define C_BLACK   "\x1b[30m"
#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_WHITE   "\x1b[37m"
#define C_RESET   "\x1b[0m"

// FG Intense Colors
#define C_IBLACK   "\x1b[30;1m"
#define C_IRED     "\x1b[31;1m"
#define C_IGREEN   "\x1b[32;1m"
#define C_IYELLOW  "\x1b[33;1m"
#define C_IBLUE    "\x1b[34;1m"
#define C_IMAGENTA "\x1b[35;1m"
#define C_ICYAN    "\x1b[36;1m"
#define C_IWHITE   "\x1b[37;1m"

// BG Colors
#define BGC_BLACK   "\x1b[40m"
#define BGC_RED     "\x1b[41m"
#define BGC_GREEN   "\x1b[42m"
#define BGC_YELLOW  "\x1b[43m"
#define BGC_BLUE    "\x1b[44m"
#define BGC_MAGENTA "\x1b[45m"
#define BGC_CYAN    "\x1b[46m"
#define BGC_WHITE   "\x1b[47m"

// BG Intense Colors
#define BGC_IBLACK   "\x1b[40;1m"
#define BGC_IRED     "\x1b[41;1m"
#define BGC_IGREEN   "\x1b[42;1m"
#define BGC_IYELLOW  "\x1b[43;1m"
#define BGC_IBLUE    "\x1b[44;1m"
#define BGC_IMAGENTA "\x1b[45;1m"
#define BGC_ICYAN    "\x1b[46;1m"
#define BGC_IWHITE   "\x1b[47;1m"

// Format defs
#define BLINK_SLOW "\x1b[5m"
#define BLINK_RAPID "\x1b[6m"

// Command defs
#define CLEAR_CONSOLE   "\x1b[2J"


// Clear console
void con_clear();

// Draw buffer to screen
void con_draw_buffer( char * buff, int buff_size);

// Set colour
void con_set_color( char* color);

// Draw coloured text
void con_draw_color( char* text, char* color);

// Set blink
void con_blink( char* blink_speed);

// Move cursor
void con_pos_cursor( unsigned int x, unsigned int y);

// Hide cursor
void nocursor();

// Reset video
void reset_video();

// Show cursor
void showcursor();

#endif
