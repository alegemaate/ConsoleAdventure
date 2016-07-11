#include <time.h>
#include <conio.h>
#include <stdlib.h>

#include "conlib.h"

// Defs for map size
#define MAP_WIDTH 160*1
#define MAP_HEIGHT 40*1
#define MAP_SIZE (MAP_WIDTH * MAP_HEIGHT)

#define SCREEN_WIDTH MAP_WIDTH/1
#define SCREEN_HEIGHT MAP_HEIGHT/1
#define SCREEN_SIZE (SCREEN_WIDTH * SCREEN_HEIGHT)

#define NUMBER_OF_TILES 20

// Declare booleans (bcs c doesnt know what they are)
typedef enum {
  FALSE, TRUE
} bool;

// Tile types
typedef struct {
  unsigned char image;
  bool passable;
  char* color;
  char* bg_color;
} tile;

// Tile names (for easy reference)
enum tile_names{
  none,
  wall,
  wall2,
  floor,
  character,
  enemy,
  scroll,
  weapon,
  armour,
  coin
};

tile tiles[NUMBER_OF_TILES];

// Character
int character_x = 3;
int character_y = 3;
int character_money = 0;

int scroll_x = 0;
int scroll_y = 0;

// Menu screen
unsigned char main_map[MAP_WIDTH][MAP_HEIGHT] = {{0}};
unsigned char main_map_old[MAP_WIDTH][MAP_HEIGHT] = {{0}};
unsigned char main_map_items[MAP_WIDTH][MAP_HEIGHT] = {{0}};
unsigned char main_map_visibility[MAP_WIDTH][MAP_HEIGHT] = {{0}};

// Functions
void draw();

// Request redrawing map
bool request_redraw = TRUE;
bool request_step = TRUE;

// Buffer for quick draws
unsigned char buffer[SCREEN_SIZE + 1];

// Random function
int random( int min, int max){
  int random_number = (rand() % (max - min)) + min;
  return random_number;
}

// Check sign
int sign(int x) {
    return (x > 0) - (x < 0);
}

// Make a rectangle
void generate_rectangle( unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fill, unsigned char border, unsigned char map_array[MAP_WIDTH][MAP_HEIGHT]){
  int i, t;
  for( i = x; i < x + width; i++){
    for( t = y; t < y + height; t++){
      if( i > x && t > y && i < x + width - 1 && t < y + height - 1 )
        map_array[i][t] = fill;
      else
        map_array[i][t] = border;

    }
  }
}

// Place tile if none
void generate_checked( unsigned int x, unsigned int y, unsigned char fill){
  if( main_map[x][y] == none)
    main_map[x][y] = fill;
}

// Make a line
void generate_line( unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned char fill, unsigned char border){
  // Temps
  int t_x1 = x1;
  int t_x2 = x2;
  int t_y1 = y1;
  int t_y2 = y2;

  char dir_x = sign( x1 - x2);
  char dir_y = sign( y1 - y2);

  // X pos
  while( t_x1 != t_x2){
    // Increase
    t_x1 += sign( t_x2 - t_x1);

    // Fill
    main_map[t_x1][t_y1] = fill;

    // Border
    generate_checked( t_x1, t_y1 + 1,  border);
    generate_checked( t_x1, t_y1 - 1,  border);

    // Last one! Corner
    if( t_x1 == t_x2){
      generate_checked( t_x1 - dir_x, t_y1,  border);
      generate_checked( t_x1 - dir_x, t_y1 + dir_y,  border);
    }
  }

  // Y pos
  while( t_y1 != t_y2){
    // Increase
    t_y1 += sign( t_y2 - t_y1);

    // Fill
    main_map[t_x1][t_y1] = fill;

    // Border
    generate_checked( t_x1 + 1, t_y1,  border);
    generate_checked( t_x1 - 1, t_y1,  border);
  }
}

// Check if area contains only 'x' and is within bounds
bool check_rectangle( unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char fill){
  // Check bounds
  if( x + width > MAP_WIDTH -1 || y + height > MAP_HEIGHT - 1)
    return FALSE;

  // Check fill
  int i, t;
  for( i = x; i < x + width; i++){
    for( t = y; t < y + height; t++){
      if( main_map[i][t] != fill)
        return FALSE;
    }
  }
  return TRUE;
}

// Generate map
void generate(){
  // Pass 1 - Fill map with nothing
  int i, t;
  for( i = 0; i < MAP_WIDTH; i++){
    for( t = 0; t < MAP_HEIGHT; t++){
      main_map[i][t] = none;
    }
  }

  // Pass 2 - Rooms
  unsigned int rect_x = 0;
  unsigned int rect_y = 0;

  unsigned int rooms_to_attempt = 60*2;

  for( i = 0; i < rooms_to_attempt; i++){
    // Choose randoms spot
    unsigned int random_x = random( 0, MAP_WIDTH - 1);
    unsigned int random_y = random( 0, MAP_HEIGHT - 1);

    // Check if on none
    if( main_map[random_x][random_y] == none){
      unsigned int random_width = random( 4, 30);
      unsigned int random_height = random( 4, 30);

      // Rect time
      if( check_rectangle( random_x, random_y, random_width, random_height, none)){
        generate_rectangle( random_x, random_y, random_width, random_height, floor, wall, main_map);

        // Make line to next rectangle
        if( rect_x != 0){
          generate_line( random_x + random( 1, random_width - 1), random_y + random( 1, random_height - 1), rect_x, rect_y, floor, wall);
        }

        // Store the new coords
        rect_x = random_x + random( 1, random_width - 1);
        rect_y = random_y + random( 1, random_height - 1);
      }
    }
  }

  // Pass 3 - Halls
  for( i = 1; i < MAP_WIDTH - 1; i++){
    for( t = 1; t < MAP_HEIGHT - 1; t++){
      if( (main_map[i - 1][t] == wall2 && main_map[i + 1][t] == wall2) ||
          (main_map[i][t - 1] == wall2 && main_map[i][t + 1] == wall2)){
        main_map[i][t] = wall2;
      }
    }
  }

  // Pass 4 - Items
  for( i = 1; i < MAP_WIDTH - 1; i++){
    for( t = 1; t < MAP_HEIGHT - 1; t++){
      if( main_map[i][t] == floor){
        int random_num = random( 0, 255);

        if( random_num == 0)
          main_map[i][t] = enemy;
        else if( random_num == 1)
          main_map[i][t] = scroll;
        else if( random_num == 2)
          main_map[i][t] = weapon;
        else if( random_num == 3)
          main_map[i][t] = armour;
        else if( random_num == 4)
          main_map[i][t] = coin;
      }
    }
  }

  // Player in random spot
  while( 1){
    unsigned int random_x = random( 0, MAP_WIDTH - 1);
    unsigned int random_y = random( 0, MAP_HEIGHT - 1);

    if( main_map[random_x][random_y] == floor){
      character_x = random_x;
      character_y = random_y;
      break;
    }
  }

  // Copy map to old map
  for( i = 0; i < MAP_WIDTH; i++){
    for( t = 0; t < MAP_HEIGHT; t++){
      main_map_old[i][t] = main_map[i][t];
    }
  }

  // NO visibility
  for( i = 0; i < MAP_WIDTH; i++){
    for( t = 0; t < MAP_HEIGHT; t++){
      main_map_visibility[i][t] = 1;
    }
  }
}

// Make tiles
void make_tiles(){
  // Default
  int i;
  for( i = 0; i < NUMBER_OF_TILES; i++){
    tiles[i].image = '?';
    tiles[i].passable = FALSE;
    tiles[i].color = C_WHITE;
    tiles[i].bg_color = BGC_BLACK;
  }

  // Filler
  tiles[none].image = ' ';

  // Wall 1
  tiles[wall].image = 178;
  tiles[wall].color = C_YELLOW;

  // Wall 2
  tiles[wall2].image = 177;
  tiles[wall2].color = C_IWHITE;

  // Floor
  tiles[floor].image = '.';
  tiles[floor].passable = TRUE;
  tiles[floor].color = C_IBLACK;

  // Character
  tiles[character].image = '@';
  tiles[character].color = C_IWHITE;
  tiles[character].bg_color = BGC_IGREEN;

  // Enemy
  tiles[enemy].image = 'E';
  tiles[enemy].color = C_WHITE;
  tiles[enemy].bg_color = BGC_IRED;

  // Scroll
  tiles[scroll].image = 'S';
  tiles[scroll].color = C_WHITE;

  // Weapon
  tiles[weapon].image = '/';
  tiles[weapon].color = C_WHITE;

  // Armour
  tiles[armour].image = 'A';
  tiles[armour].color = C_WHITE;

  // Coin
  tiles[coin].image = 'C';
  tiles[coin].color = C_YELLOW;

}

// Setup
void init(){
  // Screen size
  char temp_string[128];
  sprintf(temp_string, "%s %i %s %i", "mode ", SCREEN_WIDTH, ",", SCREEN_HEIGHT + 5);
  system(temp_string);

  // Randomize according to the time
  srand(time(NULL));

  // Init tiles
  make_tiles();

  // Generate map
  generate();
}

// Draw part or all of map to screen
void draw( int x_min, int y_min, int x_max, int y_max, bool full){
  // Draws map within range
  int i, t, k;
  for( k = 0, t = y_min; t < y_max; t++){
    for( i = x_min; i < x_max; i++, k++){
      // Check if in bounds
      if( i >= 0 && t >= 0 && i < MAP_WIDTH && t < MAP_HEIGHT){
        // Set pos!
        con_pos_cursor( i - scroll_x + 1, t - scroll_y + 1);

        if( main_map_visibility[i][t] == 1){
          // Foreground color
          con_set_color( tiles[main_map[i][t]].color);

          // Background color
          con_set_color( tiles[main_map[i][t]].bg_color);
        }
        else{
          // Foreground color
          con_set_color( C_BLACK);

          // Background color
          con_set_color( C_BLACK);
        }

        // Draw tile
        printf( "%c", tiles[main_map[i][t]].image);
      }
    }
  }

  // Line to separate game
  if ( full == TRUE){
    con_set_color( C_IWHITE);
    con_set_color( BGC_IWHITE);
    con_pos_cursor( 0, SCREEN_HEIGHT + 1);
    for( i = 0; i < SCREEN_WIDTH; i++){
      printf( "_");
    }
  }

  // Print character coords
  con_set_color( C_RED);
  con_set_color( BGC_BLACK);
  con_pos_cursor( 3, SCREEN_HEIGHT + 2);
  printf( "x:%i y:%i scrollx:%i scrolly:%i\n", character_x, character_y, scroll_x, scroll_y);
}

// Scan map for changes, if it finds, update that section
void scan_map(){
  int i, t;
  for( i = scroll_x; i < SCREEN_WIDTH + scroll_x; i++){
    for( t = scroll_y; t < SCREEN_HEIGHT + scroll_y; t++){
      // Needs refresh?
      if( main_map[i][t] != main_map_old[i][t]){
        main_map_old[i][t] = main_map[i][t];
        draw( i, t, i + 1, t + 1, FALSE);
      }
    }
  }
}

// 1 Step
void step(){
  // Update enemies
  int i, t;
  for( i = 1; i < MAP_WIDTH - 1; i++){
    for( t = 1; t < MAP_HEIGHT - 1; t++){
      if( main_map[i][t] == enemy){
        char random_direction = random( 0, 5);

        // Change in position
        char delta_x = 0;
        char delta_y = 0;

        // Arrow left
        if( random_direction == 0)
          delta_x = -1;
        // Arrow right
        else if( random_direction == 1)
          delta_x = +1;
        // Arrow up
        else if( random_direction == 2)
          delta_y = -1;
        // Arrow down
        else if( random_direction == 3)
          delta_y = +1;

        // Check if you can move there
        if( tiles[main_map[i + delta_x][t + delta_y]].passable == TRUE){
          // Move enemy
          main_map[i + delta_x][t + delta_y] = main_map[i][t];

          // Draw back over old spot
          main_map[i][t] = floor;
        }
      }
    }
  }

  // Off screen scrolling
  if( character_x - scroll_x > SCREEN_WIDTH - 1){
    scroll_x += SCREEN_WIDTH;
    request_redraw = TRUE;
  }
  else if( character_x - scroll_x < 0){
    scroll_x -= SCREEN_WIDTH;
    request_redraw = TRUE;
  }
  if( character_y - scroll_y > SCREEN_HEIGHT - 1){
    scroll_y += SCREEN_HEIGHT;
    request_redraw = TRUE;
  }
  else if(character_y - scroll_y < 0){
    scroll_y -= SCREEN_HEIGHT;
    request_redraw = TRUE;
  }

  // Place character
  main_map[character_x][character_y] = character;

  // Update visibility
  for( i = character_x - 3; i < character_x + 3; i++){
    for( t = character_y - 3; t < character_y + 3; t++){
      // Check bounds
      if( i >= 0 && t >= 0 && i < MAP_WIDTH && t < MAP_HEIGHT){
        // Is it already shown
        if( main_map_visibility[i][t] == 0){
          main_map_visibility[i][t] = 1;
          draw( i, t, i+1, t+1, FALSE);
        }
      }
    }
  }

  // Redraw discrepencies
  scan_map();
}

// Input
void input(){
  // Kbd is used
  if(kbhit()){
    // Stores key just pressed
    unsigned char key_pressed = getch();

    // Genearate map
    if ( key_pressed == 'r'){
      generate();
      request_redraw = TRUE;
    }

    // Arrow Keys
    else if( key_pressed == 224){
      // Next in buffer
      char key_pressed = getch();

      // Change in position
      char delta_x = 0;
      char delta_y = 0;

      // Arrow left
      if( key_pressed == 75)
        delta_x = -1;
      // Arrow right
      else if( key_pressed == 77)
        delta_x = +1;
      // Arrow up
      else if( key_pressed == 72)
        delta_y = -1;
      // Arrow down
      else if( key_pressed == 80)
        delta_y = +1;

      // Check if you can move there
      if( tiles[main_map[character_x + delta_x][character_y + delta_y]].passable == TRUE){
        // Draw back over old spot
        main_map[character_x][character_y] = floor;

        // Move
        character_x += delta_x;
        character_y += delta_y;

        request_step = TRUE;
      }
    }
  }
}

// Update logic
void update(){
  // Input
  input();

  // Draw full map
  if( request_redraw == TRUE){
    request_redraw = FALSE;
    draw( scroll_x, scroll_y, SCREEN_WIDTH + scroll_x, SCREEN_HEIGHT + scroll_y, TRUE);
  }

  // Do a step
  if( request_step == TRUE){
    request_step = FALSE;
    step();
  }
}

// Main
int main(){
  // Setup our game
  init();

  // Run forever
  while(1)
    update();

  return 0;
}
