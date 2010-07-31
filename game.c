#include <string.h>
#include <stdlib.h>
#include "game.h"

int
move_blocks(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks, BLOCK_MOV mov)
{
    int r, c;
    /*
      Checks that the movement doesn't confict with anything,
      specifically is not out of the grid and it doesn't go over
      another block in the grid.
    */

    switch(mov)
    {
    case LEFT:
	for (r = 0; r < a_blocks->rows; r++)
	    for (c = 0; c < a_blocks->cols; c++)
		if (a_blocks->bs[r][c] && // If there is a block, check
		    ((a_blocks->pos.col + c < 1) || // That the block is in the grid
		     // And that the block at the left in the grid is empty.
		     grid[a_blocks->pos.row + r][a_blocks->pos.col + c - 1]))
		    return(0);
	a_blocks->pos.col--;
	break;
    case RIGHT:
	for (r = 0; r < a_blocks->rows; r++)
	    for (c = 0; c < a_blocks->cols; c++)
		if (a_blocks->bs[r][c] &&
		    ((a_blocks->pos.col + c >= GRID_COLS - 1) ||
		     grid[a_blocks->pos.row + r][a_blocks->pos.col + c + 1]))
		    return(0);
	a_blocks->pos.col++;
	break;
    case UP:
	// Don't think I'll ever use that...
	break;
    case DOWN:
	for (r = 0; r < a_blocks->rows; r++)
	    for (c = 0; c < a_blocks->cols; c++)
		if (a_blocks->bs[r][c] &&
		    ((a_blocks->pos.row + r >= GRID_ROWS - 1) ||
		     grid[a_blocks->pos.row + r + 1][a_blocks->pos.col + c]))
		    return(0);
	a_blocks->pos.row++;
	break;
    default:
	break;
    }
    return(1);
}

void
generate_a_blocks(free_blocks *a_blocks)
{
    // Empty the array
    memset(a_blocks->bs, 0, sizeof(a_blocks->bs));

    int new_block = rand() % 7;
    a_blocks->pos.row = 0;
    a_blocks->pos.col = 3;
    switch(new_block)
    {
    case I:
	a_blocks->rows = 1;
	a_blocks->cols = 4;
	a_blocks->bs[0][0] = 1;
	a_blocks->bs[0][1] = 1;
	a_blocks->bs[0][2] = 1;
	a_blocks->bs[0][3] = 1;
	break;
    case J:
	a_blocks->rows = 2;
	a_blocks->cols = 3;
	a_blocks->bs[0][0] = 2;
	a_blocks->bs[0][1] = 2;
	a_blocks->bs[0][2] = 2;
	a_blocks->bs[1][2] = 2;
	break;
    case L:
	a_blocks->rows = 2;
	a_blocks->cols = 3;
	a_blocks->bs[0][0] = 3;
	a_blocks->bs[1][0] = 3;
	a_blocks->bs[0][1] = 3;
	a_blocks->bs[0][2] = 3;
	break;
    case O:
	a_blocks->pos.col = 4;
	a_blocks->rows = 2;
	a_blocks->cols = 2;
	a_blocks->bs[0][0] = 4;
	a_blocks->bs[0][1] = 4;
	a_blocks->bs[1][0] = 4;
	a_blocks->bs[1][1] = 4;
	break;
    case S:
	a_blocks->rows = 2;
	a_blocks->cols = 3;
	a_blocks->bs[1][0] = 5;
	a_blocks->bs[0][1] = 5;
	a_blocks->bs[1][1] = 5;
	a_blocks->bs[0][2] = 5;
	break;
    case T:
	a_blocks->rows = 2;
	a_blocks->cols = 3;
	a_blocks->bs[0][0] = 6;
	a_blocks->bs[0][1] = 6;
	a_blocks->bs[1][1] = 6;
	a_blocks->bs[0][2] = 6;
	break;
    case Z:
	a_blocks->rows = 2;
	a_blocks->cols = 3;
	a_blocks->bs[0][0] = 7;
	a_blocks->bs[0][1] = 7;
	a_blocks->bs[1][1] = 7;
	a_blocks->bs[1][2] = 7;
	break;
    default:
	break;
    }
}

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks)
{
    int c, r;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (a_blocks->bs[r][c])
		grid[a_blocks->pos.row + r][a_blocks->pos.col + c] = a_blocks->bs[r][c];
}

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks, int clockwise)
{
    free_blocks *new_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    
    new_blocks->cols = a_blocks->rows;
    new_blocks->rows = a_blocks->cols;
    new_blocks->pos.row = a_blocks->pos.row;
    new_blocks->pos.col = a_blocks->pos.col + ((a_blocks->cols - a_blocks->rows) / 2);

    int r, c;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (clockwise)
		new_blocks->bs[c][new_blocks->cols - 1 - r] = a_blocks->bs[r][c];
	    else
		new_blocks->bs[new_blocks->rows - 1 - c][r] = a_blocks->bs[r][c];

    memcpy(a_blocks, new_blocks, sizeof(free_blocks));
    free(new_blocks);

    return(1);
}
