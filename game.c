#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "graphics.h"
#include "animations.h"

// The active blocks
free_blocks *a_blocks = NULL;
free_blocks *next_a_blocks = NULL;

// The timer to time the falling of pieces
Uint32 fall_timer;
Uint32 fall_interval;

int mov_down;

// The level of the game
int level = 1;

// The timer for the leveling up
Uint32 level_timer;
Uint32 level_interval = LEVEL_INTERVAL;

void
init_game()
{
    // The interval in wich pieces fall
    fall_interval = FALL_INTERVAL;

    a_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    a_blocks->rows = 0;
    a_blocks->cols = 0;
    next_a_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    next_a_blocks->rows = 0;
    next_a_blocks->cols = 0;
}

void
start_game()
{
    // Generate the blocks
    generate_a_blocks(a_blocks, rand() % 7);
    generate_a_blocks(next_a_blocks, rand() % 7);

    // Set the right column
    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

    // Start the timer
    fall_timer = SDL_GetTicks();

    // Set the down movement to 0
    mov_down = 0;

    // Start the level timer
    level_timer = SDL_GetTicks();
}

int
move_blocks(int grid[GRID_ROWS][GRID_COLS],
	    BLOCK_MOV mov)
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
		if (a_blocks->bs[r][c] &&           // If there is a block, check
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
generate_a_blocks(free_blocks *f_blocks,
		  int new_block)
{
    // Empty the array
    memset(f_blocks->bs, 0, sizeof(f_blocks->bs));

    f_blocks->pos.row = 0;
    f_blocks->pos.col = 0;

    // Tetrominoes...
    switch(new_block)
    {
    case I:
	f_blocks->rows = 1;
	f_blocks->cols = 4;
	f_blocks->bs[0][0] = 1;
	f_blocks->bs[0][1] = 1;
	f_blocks->bs[0][2] = 1;
	f_blocks->bs[0][3] = 1;
	break;
    case J:
	f_blocks->rows = 2;
	f_blocks->cols = 3;
	f_blocks->bs[0][0] = 2;
	f_blocks->bs[0][1] = 2;
	f_blocks->bs[0][2] = 2;
	f_blocks->bs[1][2] = 2;
	break;
    case L:
	f_blocks->rows = 2;
	f_blocks->cols = 3;
	f_blocks->bs[0][0] = 3;
	f_blocks->bs[1][0] = 3;
	f_blocks->bs[0][1] = 3;
	f_blocks->bs[0][2] = 3;
	break;
    case O:
	f_blocks->rows = 2;
	f_blocks->cols = 2;
	f_blocks->bs[0][0] = 4;
	f_blocks->bs[0][1] = 4;
	f_blocks->bs[1][0] = 4;
	f_blocks->bs[1][1] = 4;
	break;
    case S:
	f_blocks->rows = 2;
	f_blocks->cols = 3;
	f_blocks->bs[1][0] = 5;
	f_blocks->bs[0][1] = 5;
	f_blocks->bs[1][1] = 5;
	f_blocks->bs[0][2] = 5;
	break;
    case T:
	f_blocks->rows = 2;
	f_blocks->cols = 3;
	f_blocks->bs[0][0] = 6;
	f_blocks->bs[0][1] = 6;
	f_blocks->bs[1][1] = 6;
	f_blocks->bs[0][2] = 6;
	break;
    case Z:
	f_blocks->rows = 2;
	f_blocks->cols = 3;
	f_blocks->bs[0][0] = 7;
	f_blocks->bs[0][1] = 7;
	f_blocks->bs[1][1] = 7;
	f_blocks->bs[1][2] = 7;
	break;
    default:
	break;
    }
}

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS])
{
    int c, r;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (a_blocks->bs[r][c]) // If there is a block, put it on the grid
		grid[a_blocks->pos.row + r][a_blocks->pos.col + c] = a_blocks->bs[r][c];
}

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
	      int clockwise)
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
has_lost(int grid[GRID_ROWS][GRID_COLS])
{
    int c, r;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (a_blocks->bs[r][c] && 
		grid[a_blocks->pos.row + r][a_blocks->pos.col + c])
		return(1);
    return(0);
}

int
update_grid(int grid[GRID_ROWS][GRID_COLS],
	    FPSmanager *fpsmanager)
{
    int cleared_rows[4]; // Keeps track of the complete rows
    int cleared_row, grid_changed; // Useful bools
    int counter;
    int new_grid[GRID_ROWS][GRID_COLS];
    int score = 0;

    // Makes a copy of the old grid to work on
    memcpy(new_grid, grid, sizeof(new_grid));

    int r, c;

    counter = 0; grid_changed = 0;
    // Set the grid rows to 0
    for (r = 0; r < GRID_ROWS; r++)
    {
	cleared_row = 1;
	for (c = 0; c < GRID_COLS; c++)
	    // If one cell is not 0, then the row is not cleared
	    if (!new_grid[r][c])
	    {
		cleared_row = 0;
		break;
	    }

	// If the row is cleared
	if (cleared_row)
	{
	    grid_changed = 1;
	    cleared_rows[counter] = r; // Remember it
	    counter++;
	    // Set it to 0 in the grid copy for the animation
	    memset(new_grid[r], 0, sizeof(new_grid[r]));
	}
    }
    
    // If 1 or more row were cleared
    if (grid_changed)
    {
	// Animation
	blink_grid(new_grid, grid, fpsmanager);
	
	// Remove cleared rows, and calculate score
	int i;
	score = 4;
	for (i = 0; i < counter; i++)
	{
	    // Shift the row down, starting from the cleared row
	    for (r = cleared_rows[i]; r > 0; r--)
		memcpy(grid[r], grid[r - 1], sizeof(grid[0]));

	    // Set the topmost row to 0
	    memset(grid[0], 0, sizeof(grid[0]));
	}
    }

    // Calculate score
    switch(counter)
    {
    case 0:
	return(0);
    case 1:
	return(SCORE_SINGLE);
    case 2:
	return(SCORE_DOUBLE);
    case 3:
	return(SCORE_TRIPLE);
    case 4:
	return(SCORE_TETRIS);
    default:
	return(0);
    }
}

int
game_playing(GAME_STATE *game_state,
	     SDL_Event event,
	     int grid[GRID_ROWS][GRID_COLS],
	     int *score,
	     FPSmanager *fpsmanager)
{
    while (SDL_PollEvent(&event))
    {
	switch (event.type)
	{
	case SDL_QUIT:
	    *game_state = QUIT;
	    break;
	case SDL_KEYDOWN:
	    switch (event.key.keysym.sym)
	    {
	    case SDLK_LEFT:
		move_blocks(grid, LEFT);
		break;
	    case SDLK_RIGHT:
		move_blocks(grid, RIGHT);
		break;
	    case SDLK_DOWN:
		/*
		  The DOWN movement is different, since we want to keep
		  going if the user keeps the down key pressed. So we
		  set mov_down and we unset it when the user releases the
		  key.
		*/
		mov_down = 1;
		break;
	    case SDLK_UP:
		rotate_blocks(grid, 1);
		break;
	    case SDLK_SPACE:
		rotate_blocks(grid, 0);
		break;
	    case SDLK_p:
		*game_state = PAUSED;
		break;
	    default:
		break;
	    }
	    break;
	case SDL_KEYUP:
	    switch (event.key.keysym.sym)
	    {
	    case SDLK_DOWN:
		mov_down = 0;
		break;
	    default:
		break;
	    }
	    break;
	}
    }
    // If enough time has passed, level up
    if (SDL_GetTicks() - level_timer > LEVEL_INTERVAL && level < MAX_LEVEL)
    {
	level++;
	fall_interval = FALL_INTERVAL - (FALL_INTERVAL / MAX_LEVEL * level);
	level_timer = SDL_GetTicks();
    }

    /*
      If the user is pressing the down key, or if
      enough time has passed, move the pieces down.
    */
    int enough_time = SDL_GetTicks() - fall_timer > fall_interval;
    if (enough_time || mov_down)
    {
	/*
	  If we can move down, good, if we can't, generate new
	  active blocks.
	*/
	if (!move_blocks(grid, DOWN) && enough_time)
	{
	    // Put the blocks on the grid
	    blocks_on_grid(grid);

	    // Assign the score for the drop
	    *score += DROP_SCORE;

	    // Check if there are some complete rows, and update the score
	    *score += update_grid(grid, fpsmanager);

	    // Copy the blocks planned to the active blocks
	    copy_free_blocks(a_blocks, next_a_blocks);

	    // Set the right column
	    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

	    // Check if the user has lost
	    if (has_lost(grid))
	    {
		// Animation
		game_over(grid, fpsmanager);
		*game_state = LOST;
		return(1);
	    }

	    // Generate new blocks
	    generate_a_blocks(next_a_blocks, rand() % 7);
	}
	if (enough_time)
	{
	    fall_timer = SDL_GetTicks(); // Reset timer
	}
    }

    if (!draw_game_playing(grid, a_blocks, next_a_blocks, *score, level))
	return(0);

    return(1);
}

int
game_paused(GAME_STATE *game_state,
	    SDL_Event event)
{
    while (SDL_PollEvent(&event))
    {
	switch (event.type)
	{
	case SDL_QUIT:
	    *game_state = QUIT;
	    break;
	case SDL_KEYDOWN:
	    if (event.key.keysym.sym == SDLK_p)
		*game_state = PLAYING;
	}
    }

    if (!draw_game_paused())
	return(0);

    return(1);
}

int
game_lost(int grid[GRID_ROWS][GRID_COLS],
	  GAME_STATE *game_state,
	  SDL_Event event)

{
    int r;
    while (SDL_PollEvent(&event))
    {
	switch (event.type)
	{
	case SDL_QUIT:
	    *game_state = QUIT;
	case SDL_KEYDOWN:
	    for (r = 0; r < GRID_ROWS; r++)
		memset(grid[r], 0, sizeof(grid[r])); // Empty the grid
	    start_game();
	    *game_state = PLAYING;
	}
    }

    return(1);
}


void
copy_free_blocks(free_blocks *dest,
		 free_blocks *src)
{
    dest->pos = src->pos;
    dest->rows = src->rows;
    dest->cols = src->cols;
    int r;
    for (r = 0; r < src->rows; r++)
	memcpy(dest->bs[r], src->bs[r], sizeof(int) * src->cols);
}
