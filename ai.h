#ifndef _ai_h
#define _ai_h

#include "game.h"

typedef struct {
    int direction;
    int steps;
} move;

// Function to assign scores to the grid
double
evaluate_grid(int orig_grid[GRID_ROWS][GRID_COLS],
	      free_blocks *a_blocks);

#endif
