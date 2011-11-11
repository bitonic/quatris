#ifndef _blocks_h
#define _blocks_h

#include "conf.h"

typedef struct {
  int col;
  int row;
} block_pos;

typedef struct {
  block_pos pos;
  int color;
  int type;
  int rows;
  int cols;
  int bs[4][4];
} free_blocks;

typedef enum {
  LEFT, RIGHT, UP, DOWN
} BLOCK_MOV;

typedef enum {
  I, J, L, O, S, T, Z
} BLOCK_TYPES;

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

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
              free_blocks *blocks,
              int clockwise);

// Puts the active tetromino in the grid
void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS],
               free_blocks *blocks);

#endif
