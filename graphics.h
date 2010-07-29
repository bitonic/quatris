#ifndef _graphics_h
#define _graphics_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "conf.h"
#include "game.h"

// The number of colors
#define COLORS 5
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
    RED = 1, GREEN, CYAN, PURPLE, VIOLET
} BLOCK_COLORS;

void
draw_block(SDL_Rect *type, SDL_Surface *dest, int x, int y,
	   SDL_Surface *blocks);

void
draw_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	  SDL_Surface *blocks, SDL_Rect *block_colors[COLORS]);

void
draw_a_blocks(active_blocks *a_blocks, SDL_Surface *dest,
	      SDL_Surface *blocks, SDL_Rect *block_colors[COLORS]);
void
empty_grid(int *grid[GRID_ROWS]);

#endif
