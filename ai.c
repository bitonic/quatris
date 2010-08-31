/*
  The AI is taken from the code by Colin Fahey (www.colinfahey.com)
  who got his code from Pierre Dellacherie (Email: ellache@club-internet.frd)
 */

#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"
#include "ai.h"

// Helper functions

// Returns how many blocks would be eliminated if the
// free blocks would be put on the grid.
int
count_eliminated_blocks(int orig_grid[GRID_ROWS][GRID_COLS],
			free_blocks *orig_blocks)
{
    int grid[GRID_ROWS][GRID_COLS];
    memcpy(grid, orig_grid, sizeof(grid));

    free_blocks *blocks = (free_blocks *) malloc(sizeof(free_blocks));
    memcpy(blocks, orig_blocks, sizeof(free_blocks));

    int eliminated_blocks = 0, r, c;
    // Set the color to -1
    blocks->color = -1;

    // Put blocks on grid
    blocks_on_grid(grid, blocks);

    // Update grid
    update_grid(grid, NULL);
    // Count all the -1s
    eliminated_blocks = 4;
    for (r = 2; r < GRID_ROWS; r++)
	for (c = 0; c < GRID_COLS; c++)
	    if (grid[r][c] == -1)
		eliminated_blocks--;
    
    return(eliminated_blocks);
}

int
pile_max_height(int grid[GRID_ROWS][GRID_COLS])
{
    int r, c;
    for (r = 2; r < GRID_ROWS; r++)
	for (c = 0; c < GRID_COLS; c++)
	    if (grid[r][c])
		break;
    return(GRID_ROWS - r);
}

int
get_row_transitions(int grid[GRID_ROWS][GRID_COLS],
		    int r)
{
    int c, transitions = 0;

    for (c = 0; c < GRID_COLS; c++)
	// If a transition from occupied to unoccupied, or
	// from unoccupied to occupied, it's a transition.
	if ((grid[r][c] && !grid[r][c + 1]) ||
	    (!grid[r][c] && grid[r][c + 1]))
	    transitions++;

    // Check transition between left-exterior and column 1.
    // (NOTE: Exterior is implicitly "occupied".)
    if (!grid[r][0]) 
	transitions++;

    // Check transition between column 'mWidth' and right-exterior.
    // (NOTE: Exterior is implicitly "occupied".)
    if (!grid[r][GRID_COLS - 1])
	transitions++;

    return(transitions);
}

int
get_col_transitions(int grid[GRID_ROWS][GRID_COLS],
		    int c)
{
    int transitions = 0, r;

    // Check cell and neighbor above...
    for (r = 2; r < GRID_ROWS - 1; r++)
	// If a transition from occupied to unoccupied, or
	// from unoccupied to occupied, it's a transition.
	if ((grid[r][c] && !grid[r + 1][c]) ||
	    (!grid[r][c] && grid[r + 1][c]))
	    transitions++;

    // Check transition between bottom-exterior and row Y=1.
    // (NOTE: Bottom exterior is implicitly occupied.)

    if (!grid[GRID_ROWS - 1][c])
	transitions++;

    // Check transition between column 'mHeight' and above-exterior.
    // (NOTE: Sky above is implicitly UN-occupied.)
    if (grid[2][c]) 
	transitions++;

    return(transitions);
}

int
get_buried_holes(int grid[GRID_ROWS][GRID_COLS],
		 int c)
{
    int enable = 0, holes = 0, r;

    for (r = 2; r < GRID_ROWS; r++)
	if (grid[r][c])
	    enable = 1;
	else if (enable)
	    holes++;

    return holes;
}

int
blanks_down(int grid[GRID_ROWS][GRID_COLS],
	    int top_r,
	    int c)
{
    int blanks = 0;
    int r;

    for (r = top_r; r < GRID_ROWS; r++)
	if (grid[r][c])
	    return(blanks);
	else
	    blanks++;
    return(blanks);
}

int
get_wells(int grid[GRID_ROWS][GRID_COLS],
	  int c)
{
    int r, well_value = 0, cell_left, cell_right;

    
    for (r = 2; r < GRID_ROWS; r++)
    {
	if (c > 0)
	    cell_left = grid[r][c - 1];
	else
	    cell_left = 1;

	if (c < GRID_COLS - 1)
	    cell_right = grid[r][c + 1];
	else
	    cell_right = 1;

	if (cell_left && cell_right)
	    well_value += blanks_down(grid, r, c);
    }

    return(well_value);
}

//-------------

void
evaluate_grid(int orig_grid[GRID_ROWS][GRID_COLS],
	      free_blocks *orig_blocks,
	      int *priority,
	      double *score)
{
   
    int grid[GRID_ROWS][GRID_COLS];
    memcpy(grid, orig_grid, sizeof(grid));

    free_blocks *blocks = (free_blocks *) malloc(sizeof(free_blocks));
    memcpy(blocks, orig_blocks, sizeof(free_blocks));

    double rating = 0.0;

    int eroded_blocks = 0;
    int row_transitions = 0;
    int col_transitions = 0;
    int buried_holes = 0;
    int wells = 0;
    int pile_height = 0;

    int r, c;

    double landing_height = 0.5 *
	(((double) (GRID_ROWS - blocks->pos.row - blocks->rows)) +
	 ((double) (GRID_ROWS - blocks->pos.row)));

    int eliminated_blocks = count_eliminated_blocks(grid, blocks);
    
    // Calculate priority
    *priority = 0;
    *priority += 100 * abs(blocks->pos.col - (GRID_COLS / 2 - blocks->cols / 2));
    if (blocks->pos.col < GRID_COLS / 2 - blocks->cols / 2)
	priority += 10;

    // Put the blocks on the grid
    blocks_on_grid(grid, blocks);

    // If we lost, return a BAD score
    for (r = 0; r < 2; r++)
	for (c = 0; c < GRID_COLS; c++)
	    if (grid[r][c])
	    {
		*score = -1.0e+20;
		return;
	    }	
    
    // Calcolate eroded blocks, updating the grid
    eroded_blocks = eliminated_blocks * update_grid(grid, NULL);

    pile_height = pile_max_height(grid);

    row_transitions = 2 * (GRID_ROWS - pile_height);
    
    for (r = 2; r < pile_height; r++)
	row_transitions += get_row_transitions(grid, r);

    for (c = 0; c < GRID_COLS; c++)
    {
	col_transitions += get_col_transitions(grid, c);
	buried_holes += get_buried_holes(grid, c);
	wells += get_wells(grid, c);
    }

    rating = 0.0;
    rating += -1.0 * landing_height;
    rating += (double) eroded_blocks;
    rating += -1.0 * ((double) row_transitions);
    rating += -1.0 * ((double) col_transitions);
    rating += -4.0 * ((double) buried_holes);
    rating += -1.0 * ((double) wells);

    *score = rating;

    free(blocks);
}

ai_move
get_best_move(int grid[GRID_ROWS][GRID_COLS],
	      free_blocks *a_blocks)
{
    free_blocks *blocks1 = (free_blocks *) malloc(sizeof(free_blocks));
    free_blocks *blocks2 = (free_blocks *) malloc(sizeof(free_blocks));
    free_blocks *blocks3 = (free_blocks *) malloc(sizeof(free_blocks));

    ai_move best_move;
    memset(&best_move, 0, sizeof(ai_move));
    ai_move tmp_move;
    memset(&tmp_move, 0, sizeof(ai_move));
    double best_score = -1.0e+20;
    double tmp_score;
    int best_priority = 0;
    int tmp_priority;

    // Make a fresh copy
    memcpy(blocks1, a_blocks, sizeof(free_blocks));
    
    int rotations;
    for (rotations = 0;
	 rotations < blocks_possible_rotations(a_blocks->type);
	 rotations++)
    {
	tmp_move.rotations = rotations;
	// Rotate the block if we can. If not, break

	if (rotations)
	    if (!rotate_blocks(grid, blocks1, 1))
		break;
	
	// Move left
	memcpy(blocks2, blocks1, sizeof(free_blocks));
	do {
	    tmp_move.column = blocks2->pos.col;
	    // Make another copy
	    memcpy(blocks3, blocks2, sizeof(free_blocks));
	    // Drop the block
	    drop_blocks(grid, blocks3);
	    // Evaluate
	    evaluate_grid(grid, blocks3, &tmp_priority, &tmp_score);
	    // If it's better...
	    if (tmp_score > best_score ||
		(tmp_score == best_score && tmp_priority > best_priority))
	    {
		best_score = tmp_score;
		best_priority = tmp_priority;
		memcpy(&best_move, &tmp_move, sizeof(ai_move));
	    }
	} while (move_blocks(grid, blocks2, LEFT));

	// Move right
	memcpy(blocks2, blocks1, sizeof(free_blocks));
	while (move_blocks(grid, blocks2, RIGHT))
	{
	    tmp_move.column = blocks2->pos.col;
	    // Make another copy
	    memcpy(blocks3, blocks2, sizeof(free_blocks));
	    // Drop the block
	    drop_blocks(grid, blocks3);
	    // Evaluate
	    evaluate_grid(grid, blocks3, &tmp_priority, &tmp_score);
	    // If it's better...
	    if (tmp_score > best_score ||
		(tmp_score == best_score && tmp_priority > best_priority))
	    {
		best_score = tmp_score;
		best_priority = tmp_priority;
		memcpy(&best_move, &tmp_move, sizeof(ai_move));
	    }
	}
    }

    free(blocks1);
    free(blocks2);
    free(blocks3);

    return(best_move);
}

int
execute_ai_move(int grid[GRID_ROWS][GRID_COLS],
		free_blocks *blocks,
		ai_move *move,
		int animate,
		Uint32 *timer,
		int level)
{
    // Convert clockwise rotations to counter clockwise if necessary
    if (move->rotations > 2)
	move->rotations -= 4;
    
    // If we have to animate, move/rotate only once
    int interval = AI_ANIMATION_INTERVAL - AI_ANIMATION_INTERVAL / MAX_LEVEL * level;
    if (animate && (SDL_GetTicks() - *timer > interval))
    {
	*timer = SDL_GetTicks();

	// If we are done, drop
	if (move->rotations == 0 && move->column == blocks->pos.col)
	{
	    drop_blocks(grid, blocks);
	    return(1);
	}
	else
	{
	    // Rotate
	    if (move->rotations > 0)
	    {
		rotate_blocks(grid, blocks, 1);
		move->rotations--;
	    }
	    else if (move->rotations < 0)
	    {
		rotate_blocks(grid, blocks, 0);
		move->rotations++;
	    }
	    
	    // Move
	    if (move->column < blocks->pos.col)
		move_blocks(grid, blocks, LEFT);
	    else if (move->column > blocks->pos.col)
		move_blocks(grid, blocks, RIGHT);
	    return(0);
	}
	
    }
    else if (!animate)
    {
	// Else, do everything in one go
	while (move->rotations)
	{
	    if (move->rotations > 0)
	    {
		move->rotations--;
		rotate_blocks(grid, blocks, 1);
	    }
	    else
	    {
		move->rotations++;
		rotate_blocks(grid, blocks, 0);
	    }
	}
	
	while (move->column != blocks->pos.col)
	    if (move->column < blocks->pos.col)
		move_blocks(grid, blocks, LEFT);
	    else if (move->column > blocks->pos.col)
		move_blocks(grid, blocks, RIGHT);
	
	drop_blocks(grid, blocks);
    
	return(1);
    }
    else
	return(0);
}

// This function assumes that the free_blocks have 0 as a column.
double
get_best_move_score(int grid[GRID_ROWS][GRID_COLS],
		    free_blocks *blocks)
{
    free_blocks *blocks1 = (free_blocks *) malloc(sizeof(free_blocks));
    free_blocks *blocks2 = (free_blocks *) malloc(sizeof(free_blocks));
    free_blocks *blocks3 = (free_blocks *) malloc(sizeof(free_blocks));

    double best_score = -1.0e+20;
    double tmp_score;
    
    int tmp_priority = 0; // Dummy

    // Make a fresh copy
    memcpy(blocks1, blocks, sizeof(free_blocks));
    
    int rotations;
    for (rotations = 0;
	 rotations < blocks_possible_rotations(blocks->type);
	 rotations++)
    {
	// Rotate the block if we can. If not, break
	if (rotations)
	    if (!rotate_blocks(grid, blocks1, 1))
		break;
	
	// Move right
	memcpy(blocks2, blocks1, sizeof(free_blocks));
	while (move_blocks(grid, blocks2, RIGHT))
	{
	    // Make another copy
	    memcpy(blocks3, blocks2, sizeof(free_blocks));
	    // Drop the block
	    drop_blocks(grid, blocks3);
	    // Evaluate
	    evaluate_grid(grid, blocks3, &tmp_priority, &tmp_score);
	    // If it's better...
	    if (tmp_score > best_score)
		best_score = tmp_score;
	}
    }

    free(blocks1);
    free(blocks2);
    free(blocks3);

    return(best_score);
}

int
bastard_mode_blocks(int grid[GRID_ROWS][GRID_COLS])
{
    free_blocks *tmp_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    double blocks_scores[7];
    int return_blocks = 0;

    int i;
    for (i = 0; i < 7; i++)
    {
	generate_blocks(tmp_blocks, i);
	blocks_scores[i] = get_best_move_score(grid, tmp_blocks);
    }
    
    free(tmp_blocks);

    int prob = rand() % 100;
    if (prob <= 100)
    {
	double tmp_score;
	tmp_score = 1.0e+20;
	for (i = 0; i < 7; i++)
	    if (blocks_scores[i] < tmp_score)
	    {
		tmp_score = blocks_scores[i];
		return_blocks = i;
	    }
	return(return_blocks);
    }
    else if (prob <= 85)
    {
	double tmp_score = 1.0e+20;
	int tmp_return = 0;
	for (i = 0; i < 7; i++)
	    if (blocks_scores[i] < tmp_score)
	    {
		tmp_score = blocks_scores[i];
		return_blocks = tmp_return;
		tmp_return = i;
	    }
	return(return_blocks);
    }
    else if (prob <= 94)
    {
	return(rand() % 7);
    }
    else
    {
	printf("Best block\n");

	
	double tmp_score = -1.0e+20;
	for (i = 0; i < 7; i++)
	    if (blocks_scores[i] > tmp_score)
	    {
		tmp_score = blocks_scores[i];
		return_blocks = i;
	    }

	return(return_blocks);
    }
}
