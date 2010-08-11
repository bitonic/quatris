#ifndef _game_h
#define _game_h

#include "conf.h"
#include "blocks.h"

// Move the blocks
int
move_blocks(int grid[][GRID_COLS], free_blocks *a_blocks, BLOCK_MOV mov);

// Generate a new tetromino at the top of the grid
void
generate_a_blocks(free_blocks *a_blocks);


// Puts the active tetromino in the grid
void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks);

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks, int clockwise);

/*
int
update_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	    SDL_Surface *blocks, SDL_Rect *block_colors[7],
	    FPSmanager *fpsmanager);
*/
int
update_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	    SDL_Surface *blocks, SDL_Rect *block_colors[7],
	    FPSmanager *fpsmanager);

#endif
