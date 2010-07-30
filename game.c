#include <stdlib.h>
#include "game.h"

void
set_active_blocks(int r1, int c1,
		  int r2, int c2,
		  int r3, int c3,
		  int r4, int c4,
		  int color,
		  free_blocks *a_blocks)
{
    a_blocks->pos[0].col = c1;
    a_blocks->pos[0].row = r1;
    a_blocks->pos[1].col = c2;
    a_blocks->pos[1].row = r2;
    a_blocks->pos[2].col = c3;
    a_blocks->pos[2].row = r3;
    a_blocks->pos[3].col = c4;
    a_blocks->pos[3].row = r4;
    a_blocks->color = color;
}

int
move_blocks(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks, BLOCK_MOV mov)
{
    int i;
    /*
      Checks that the movement doesn't confict with anything,
      specifically is not out of the grid and it doesn't go over
      another block in the grid that we are not moving.
    */

    switch(mov)
    {
    case LEFT:
	for (i = 0; i < 4; i++)
	    if ((a_blocks->pos[i].col < 1) ||
		grid[a_blocks->pos[i].row][a_blocks->pos[i].col - 1])
		return(0);
	for (i = 0; i < 4; i++)
	    a_blocks->pos[i].col--;
	break;
    case RIGHT:
	for (i = 0; i < 4; i++)
	    if ((a_blocks->pos[i].col >= GRID_COLS - 1) ||
		grid[a_blocks->pos[i].row][a_blocks->pos[i].col + 1])
		return(0);
	for (i = 0; i < 4; i++)
	    a_blocks->pos[i].col++;
	break;
    case UP:
	// Don't think I'll ever use that...
	break;
    case DOWN:
	for (i = 0; i < 4; i++)
	    if ((a_blocks->pos[i].row >= GRID_ROWS - 1) ||
		grid[a_blocks->pos[i].row + 1][a_blocks->pos[i].col])
		return(0);
	for (i = 0; i < 4; i++)
	    a_blocks->pos[i].row++;
	break;
    default:
	break;
    }
    return(1);
}

void
generate_a_blocks(free_blocks *a_blocks)
{
    int new_block = rand() % 7;

    switch(new_block)
    {
    case I:
	set_active_blocks(0, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  0, GRID_COLS / 2 + 1,
			  1, a_blocks);
	break;
    case J:
	set_active_blocks(0, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  1, GRID_COLS / 2,
			  2, a_blocks);
	break;
    case L:
	set_active_blocks(1, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  3, a_blocks);
	break;
    case O:
	set_active_blocks(0, GRID_COLS / 2 - 1,
			  1, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  1, GRID_COLS / 2,
			  4, a_blocks);
	break;
    case S:
	set_active_blocks(1, GRID_COLS / 2 - 2,
			  1, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  4, a_blocks);
	break;
    case T:
	set_active_blocks(0, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 1,
			  1, GRID_COLS / 2 - 1,
			  0, GRID_COLS / 2,
			  5, a_blocks);
	break;
    case Z:
	set_active_blocks(0, GRID_COLS / 2 - 2,
			  0, GRID_COLS / 2 - 1,
			  1, GRID_COLS / 2 - 1,
			  1, GRID_COLS / 2,
			  6, a_blocks);
	break;
    }
}

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS], free_blocks *a_blocks)
{
    int i;
    for (i = 0; i < 4; i++)
	grid[a_blocks->pos[i].row][a_blocks->pos[i].col] = a_blocks->color;
}
