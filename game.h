#ifndef _game_h
#define _game_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "SDL/SDL_ttf.h"
#include "conf.h"
#include "blocks.h"

// The game state
typedef enum {
    PLAYING, PAUSED, LOST, SPLASHSCREEN, QUIT
} GAME_STATE;

void
init_game();

void
start_game(int grid[GRID_ROWS][GRID_COLS]);

/*
  GAME OPERATIONS
*/

// Move the blocks
int
move_blocks(int grid[][GRID_COLS],
	    BLOCK_MOV mov);

// Generate a new tetromino
void
generate_a_blocks(free_blocks *f_blocks,
		  int new_block);

// Puts the active tetromino in the grid
void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS]);

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
	      int clockwise);

// Checks if the player has lost
int
has_lost(int grid[GRID_ROWS][GRID_COLS]);

/*
  If there are complete rows, then it animates
  the grid and removes the rows from it.
*/
int
update_grid(int grid[GRID_ROWS][GRID_COLS],
	    FPSmanager *fpsmanager);

/*
  UTILS
*/
void
copy_free_blocks(free_blocks *dest,
		 free_blocks *src);

/*
  GAME STATES
*/

int
game_playing(GAME_STATE *game_state,
	     SDL_Event event,
	     int grid[GRID_ROWS][GRID_COLS],
	     int *score,
	     FPSmanager *fpsmanager);

int
game_paused(GAME_STATE *game_state,
	    SDL_Event event);

int
game_lost(int grid[GRID_ROWS][GRID_COLS],
	  GAME_STATE *game_state,
	  SDL_Event event);

#endif
