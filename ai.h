#ifndef _ai_h
#define _ai_h

#include "game.h"

typedef struct {
    int column;
    int rotations;
} ai_move;

#define AI_ANIMATION_INTERVAL 200

// Function to assign scores to the grid
void
evaluate_grid(int orig_grid[GRID_ROWS][GRID_COLS],
	      free_blocks *orig_blocks,
	      int *priority,
	      double *score);

ai_move
get_best_move(int grid[GRID_ROWS][GRID_COLS],
	      free_blocks *a_blocks);

int
execute_ai_move(int grid[GRID_ROWS][GRID_COLS],
		free_blocks *blocks,
		ai_move *move,
		int animate,
		Uint32 *timer,
		int level);

#endif
