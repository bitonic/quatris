#ifndef _game_h
#define _game_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "SDL/SDL_ttf.h"
#include "blocks.h"

// Defines..
#define GRID_COLS 10
#define GRID_ROWS 22
#define GRID_ACTUAL_ROWS 20

#define DROP_SCORE 6
#define SCORE_SINGLE 40
#define SCORE_DOUBLE 100
#define SCORE_TRIPLE 300
#define SCORE_TETRIS 1200
#define MAX_SCORE 999999999

#define FALL_INTERVAL 600

#define LINES_PER_LEVEL 10
#define MAX_LEVEL 15
#define MAX_LINES 999999999

// The game state
typedef enum {
    PLAYING, PAUSED, LOST, SPLASHSCREEN, QUIT
} GAME_STATE;

void
init_game();

void
start_game(int grid[GRID_ROWS][GRID_COLS]);

void
clean_up_game();


// Checks if the player has lost
int
has_lost(int grid[GRID_ROWS][GRID_COLS]);

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

/*
  BLOCKS OPERATIONS
*/

// Move the blocks
int
move_blocks(int grid[][GRID_COLS],
	    free_blocks *blocks,
	    BLOCK_MOV mov);

// Drop the current blocks
void
drop_blocks(int grid[GRID_ROWS][GRID_COLS],
	    free_blocks *blocks);

// Generate a new tetromino
void
generate_blocks(free_blocks *blocks,
		int new_block);

// Returns the number of possible orientation of a tetromino
int
blocks_possible_rotations(int block);

// Puts the active tetromino in the grid
void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS],
	       free_blocks *blocks);

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
	      free_blocks *blocks,
	      int clockwise);

/*
  If there are complete rows, then it animates
  the grid and removes the rows from it.
  Returns the number of completed rows
*/
int
update_grid(int grid[GRID_ROWS][GRID_COLS],
	    FPSmanager *fpsmanager);
#endif
