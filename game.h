#ifndef _game_h
#define _game_h

#include "conf.h"

typedef struct {
    int col;
    int row;
} block_pos;

typedef struct {
    int color;
    block_pos pos[4];
} active_blocks;

typedef enum {
    LEFT, RIGHT, UP, DOWN
} BLOCK_MOV;

void
set_active_blocks(int c1, int r1,
		  int c2, int r2,
		  int c3, int r3,
		  int c4, int r4,
		  int color,
		  active_blocks *a_blocks);

int
move_blocks(int grid[][GRID_COLS], active_blocks *a_blocks, BLOCK_MOV mov);

typedef enum {
    I, J, L, O, S, T, Z
} BLOCK_TYPES;

void
generate_a_blocks(active_blocks *a_blocks);

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS], active_blocks *a_blocks);

#endif
