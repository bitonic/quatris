#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "game.h"
#include "graphics.h"

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
    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;
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
	    {
		// Check that there is no collision
		if (new_blocks->bs[c][new_blocks->cols - 1 - r] = a_blocks->bs[r][c])
		    if (new_blocks->pos.row + new_blocks->rows - 1 - c >= GRID_ROWS ||
			new_blocks->pos.row + new_blocks->rows - 1 - c < 0 ||
			new_blocks->pos.col + r >= GRID_COLS ||
			new_blocks->pos.col + r < 0 ||
			grid[new_blocks->pos.row + new_blocks->rows - 1 - c][new_blocks->pos.col + r])
			return(0);
	    }
	    else
	    {
		// Check that there is no collision
		if (new_blocks->bs[c][new_blocks->cols - 1 - r] = a_blocks->bs[r][c])
		    if (new_blocks->pos.row + c >= GRID_ROWS ||
			new_blocks->pos.row + c < 0 ||
			new_blocks->pos.col + new_blocks->cols - 1 - r >= GRID_COLS ||
			new_blocks->pos.col + new_blocks->cols - 1 - r < 0 ||
			grid[new_blocks->pos.row + c][new_blocks->pos.col + new_blocks->cols - 1 - r])
			return(0);

		new_blocks->bs[new_blocks->rows - 1 - c][r] = a_blocks->bs[r][c];
	    }

    memcpy(a_blocks, new_blocks, sizeof(free_blocks));
    free(new_blocks);

    return(1);
}

int
update_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	    SDL_Surface *blocks, SDL_Rect *block_colors[7],
	    FPSmanager *fpsmanager)
{
    Uint32 timer, interval;
    int cleared_rows[4], cleared_row, counter, grid_changed;
    int new_grid[GRID_ROWS][GRID_COLS];

    memcpy(new_grid, grid, sizeof(new_grid));

    int r, c;

    counter = 0; grid_changed = 0;
    // Set the grid rows to 0
    for (r = 0; r < GRID_ROWS; r++)
    {
	cleared_row = 1;
	for (c = 0; c < GRID_COLS; c++)
	    if (!new_grid[r][c])
	    {
		cleared_row = 0;
		break;
	    }

	if (cleared_row)
	{
	    grid_changed = 1;
	    cleared_rows[counter] = r;
	    counter++;
	    memset(new_grid[r], 0, sizeof(new_grid[r]));
	}
    }

    if (grid_changed)
    {
	// Animate
	timer = SDL_GetTicks();
	interval = 200;
	while (SDL_GetTicks() - timer < interval)
	    draw_game(new_grid, NULL, dest, blocks, block_colors, fpsmanager);
	timer = SDL_GetTicks();
	while (SDL_GetTicks() - timer < interval)
	    draw_game(grid, NULL, dest, blocks, block_colors, fpsmanager);
	timer = SDL_GetTicks();
	while (SDL_GetTicks() - timer < interval)
	    draw_game(new_grid, NULL, dest, blocks, block_colors, fpsmanager);

	// Remove cleared rows
	int i;
	for (i = 0; i < counter; i++)
	{
	    for (r = cleared_rows[i]; r > 0; r--)
		memcpy(grid[r], grid[r - 1], sizeof(grid[0]));

	    memset(grid[0], 0, sizeof(grid[0]));
	}
    }

    return(0);
}
