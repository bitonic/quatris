#ifndef _graphics_h
#define _graphics_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "blocks.h"
#include "game.h"

#define SCREEN_WIDTH 530
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

// The length of the block side
#define BLOCK_SIZE 20
// The position of the grid
#define GRID_POS_X 20
#define GRID_POS_Y 20

// Colors
#define GRID_BGR 0x101010FF


/*
  These must be in the same order as in the 
  blocks.png image.
  The transparent blocks (TR) must be after the non-transparent ones
 */
#define COLORS_QTY 15
typedef enum {
    CYAN = 1, BLUE, ORANGE, YELLOW,
    GREEN, PURPLE, RED,
    TRCYAN, TRBLUE, TRORANGE, TRYELLOW,
    TRGREEN, TRPURPLE, TRRED,
    GREY
} BLOCK_COLORS;


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

void
clean_up_graphics();

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
draw_shadow_blocks(int grid[GRID_ROWS][GRID_COLS],
		   free_blocks *a_blocks);


void
empty_grid(int grid[GRID_ROWS][GRID_COLS]);

int
draw_game_playing(int grid[GRID_ROWS][GRID_COLS],
		  free_blocks *a_blocks,
		  free_blocks *next_a_blocks,
		  int score,
		  int level,
		  int lines,
		  int draw_shadow);

int
draw_splashscreen(int anykey);

int
draw_game_paused();

int
draw_game_lost();

#endif
