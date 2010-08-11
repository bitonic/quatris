#ifndef _graphics_h
#define _graphics_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "conf.h"
#include "blocks.h"

// The length of the block side
#define BLOCK_SIZE 20
// The position of the grid
#define GRID_POS_X 40
#define GRID_POS_Y 40

void
apply_surface(SDL_Surface *source, SDL_Rect *clip,
	      SDL_Surface *dest, int x, int y);

SDL_Surface *
load_image(char *filename);

/*
 * These must be in the same order as in the 
 * blocks.png image
 */
typedef enum {
    CYAN = 1, BLUE, ORANGE, YELLOW,
    GREEN, PURPLE, RED
} BLOCK_COLORS;

void
draw_block(SDL_Rect *type, SDL_Surface *dest, int x, int y,
	   SDL_Surface *blocks);

void
draw_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	  SDL_Surface *blocks, SDL_Rect *block_colors[7]);

void
draw_a_blocks(free_blocks *a_blocks, SDL_Surface *dest,
	      SDL_Surface *blocks, SDL_Rect *block_colors[7]);

void
empty_grid(int grid[GRID_ROWS][GRID_COLS]);

int
draw_game(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks,
	  SDL_Surface *screen, SDL_Surface *blocks_sprite,
	  SDL_Rect *blocks_colors[7], FPSmanager *fpsmanager);

#endif
