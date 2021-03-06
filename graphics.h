#ifndef _graphics_h
#define _graphics_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "blocks.h"
#include "game.h"
#include "conf.h"

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

void
draw_rectangle(SDL_Surface *dest,
               int x,
               int y,
               int w,
               int h,
               Uint32 color);

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
