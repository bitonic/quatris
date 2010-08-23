#ifndef _graphics_h
#define _graphics_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "conf.h"
#include "blocks.h"

// The length of the block side
#define BLOCK_SIZE 20
// The position of the grid
#define GRID_POS_X 40
#define GRID_POS_Y 40

// Colors
#define GRID_BGR 0x101010FF
#define GAME_PAUSED_BGR 0x00000000

void
apply_surface(SDL_Surface *source,
	      SDL_Rect *clip,
	      SDL_Surface *dest,
	      int x,
	      int y);

SDL_Surface *
load_image(char *filename);

int
init_graphics();

/*
 * These must be in the same order as in the 
 * blocks.png image
 */
typedef enum {
    CYAN = 1, BLUE, ORANGE, YELLOW,
    GREEN, PURPLE, RED, GREY
} BLOCK_COLORS;

void
draw_block(SDL_Rect *type,
	   SDL_Surface *dest,
	   int x,
	   int y);

void
draw_grid(int grid[GRID_ROWS][GRID_COLS],
	  SDL_Surface *dest);

void
draw_free_blocks(free_blocks *a_blocks,
		 SDL_Surface *dest,
		 int x, int y);

void
empty_grid(int grid[GRID_ROWS][GRID_COLS]);

int
draw_game_playing(int grid[GRID_ROWS][GRID_COLS],
		  free_blocks *a_blocks,
		  free_blocks *next_a_blocks,
		  int score,
		  int level);

int
draw_game_paused();

#endif
