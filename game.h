#ifndef _game_h
#define _game_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "conf.h"
#include "blocks.h"

// The game state
typedef enum {
    PLAYING, PAUSED, LOST, SPLASHSCREEN, QUIT
} GAME_STATE;

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

int
update_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	    SDL_Surface *blocks, SDL_Rect *block_colors[7],
	    FPSmanager *fpsmanager);

int
game_playing(GAME_STATE *game_state,
	     SDL_Surface *screen,
	     SDL_Surface *blocks_sprite,
	     SDL_Rect *block_colors[7],
	     SDL_Event event,
	     int grid[GRID_ROWS][GRID_COLS],
	     free_blocks *a_blocks,
	     Uint32 *fall_timer,
	     Uint32 *fall_interval,
	     int *mov_down,
	     FPSmanager *fpsmanager);

int
game_paused(GAME_STATE *game_state,
	    SDL_Surface *screen,
	    SDL_Event event);


#endif
