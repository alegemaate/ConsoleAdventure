#include "conlib.h"

// Clear console
void con_clear(){
  printf(CLEAR_CONSOLE);
}

// Draw buffer to screen
void con_draw_buffer( char * buff, int buff_size){
  //fwrite(buff, buff_size, 1, stdout);
  printf( buff);
}

// Move cursor
void con_pos_cursor( unsigned int x, unsigned int y){
  printf("\x1b[%i;%iH", y, x);
}

// Set colored
void con_set_color( char* color){
  printf("%s", color);
}

// Draw colored text
void con_draw_color( char* text, char* color){
  printf("%s %s %s", color, text, C_RESET);
}

// Set colored
void con_blink( char* blink_speed){
  printf("%s", blink_speed);
}

// Hide cursor
void nocursor()
{
  printf("\x1b[?25l");
}

// Reset video
void reset_video()
{
  printf("\x1b[0m");
}

// Show cursor
void showcursor()
{
  printf("\x1b[?25h");
}
