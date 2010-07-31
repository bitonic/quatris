#ifndef _game_h
#define _game_h

#include "conf.h"

typedef struct {
    int col;
    int row;
} block_pos;

typedef struct {
    block_pos pos;
    int rows;
    int cols;
    int bs[4][4];
} free_blocks;

typedef enum {
    LEFT, RIGHT, UP, DOWN
} BLOCK_MOV;

// Move the blocks
int
move_blocks(int grid[][GRID_COLS], free_blocks *a_blocks, BLOCK_MOV mov);

typedef enum {
    I, J, L, O, S, T, Z
} BLOCK_TYPES;

// Generate a new tetromino at the top of the grid
void
generate_a_blocks(free_blocks *a_blocks);


// Puts the active tetromino in the grid
void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks);

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks, int clockwise);

#endif
