#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "graphics.h"
#include "animations.h"
#include "ai.h"

// The active blocks
free_blocks *a_blocks = NULL;
free_blocks *next_a_blocks = NULL;

// The timer to time the falling of pieces
Uint32 fall_timer;
Uint32 fall_interval;

// The number of cleared lines
int lines = 0;
// The level of the game
int level = 1;

// bools to control the user's will
int mov_down, drop;

// Bool that registers wheter to draw the shadow or not
int draw_shadow;

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
start_game(int grid[GRID_ROWS][GRID_COLS])
{
    // Empty the grid
    int r;
    for (r = 0; r < GRID_ROWS; r++)
	memset(grid[r], 0, sizeof(grid[r])); // Empty the grid

    // Set cleared lines to 0
    lines = 0;

    // Generate the blocks
    generate_a_blocks(a_blocks, rand() % 7);
    generate_a_blocks(next_a_blocks, rand() % 7);

    // Set the right column
    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

    mov_down = 0;
    drop = 0;

    // Start the timer
    fall_timer = SDL_GetTicks();
}

int
move_blocks(int grid[GRID_ROWS][GRID_COLS],
	    free_blocks *blocks,
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
	for (r = 0; r < blocks->rows; r++)
	    for (c = 0; c < blocks->cols; c++)
		if (blocks->bs[r][c] &&           // If there is a block, check
		    ((blocks->pos.col + c < 1) || // That the block is in the grid
		     // And that the block at the left in the grid is empty.
		     grid[blocks->pos.row + r][blocks->pos.col + c - 1]))
		    return(0);
	blocks->pos.col--;
	break;
    case RIGHT:
	for (r = 0; r < blocks->rows; r++)
	    for (c = 0; c < blocks->cols; c++)
		if (blocks->bs[r][c] &&
		    ((blocks->pos.col + c >= GRID_COLS - 1) ||
		     grid[blocks->pos.row + r][blocks->pos.col + c + 1]))
		    return(0);
	blocks->pos.col++;
	break;
    case UP:
	// Don't think I'll ever use that...
	break;
    case DOWN:
	for (r = 0; r < blocks->rows; r++)
	    for (c = 0; c < blocks->cols; c++)
		if (blocks->bs[r][c] &&
		    ((blocks->pos.row + r >= GRID_ROWS - 1) ||
		     grid[blocks->pos.row + r + 1][blocks->pos.col + c]))
		    return(0);
	blocks->pos.row++;
	break;
    default:
	break;
    }
    return(1);
}

void
drop_blocks(int grid[GRID_ROWS][GRID_COLS],
	    free_blocks *blocks)
{
    while (move_blocks(grid, blocks, DOWN))
	;
}

void
generate_a_blocks(free_blocks *blocks,
		  int new_block)
{
    // Empty the array
    memset(blocks->bs, 0, sizeof(blocks->bs));

    blocks->pos.row = 0;
    blocks->pos.col = 0;

    blocks->type = new_block;
    // Tetrominoes...
    switch(new_block)
    {
    case I:
	blocks->rows = 1;
	blocks->cols = 4;
	blocks->color = CYAN;
	blocks->bs[0][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[0][2] = 1;
	blocks->bs[0][3] = 1;
	break;
    case J:
	blocks->rows = 2;
	blocks->cols = 3;
	blocks->color = BLUE;
	blocks->bs[0][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[0][2] = 1;
	blocks->bs[1][2] = 1;
	break;
    case L:
	blocks->rows = 2;
	blocks->cols = 3;
	blocks->color = ORANGE;
	blocks->bs[0][0] = 1;
	blocks->bs[1][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[0][2] = 1;
	break;
    case O:
	blocks->rows = 2;
	blocks->cols = 2;
	blocks->color = YELLOW;
	blocks->bs[0][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[1][0] = 1;
	blocks->bs[1][1] = 1;
	break;
    case S:
	blocks->rows = 2;
	blocks->cols = 3;
	blocks->color = GREEN;
	blocks->bs[1][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[1][1] = 1;
	blocks->bs[0][2] = 1;
	break;
    case T:
	blocks->rows = 2;
	blocks->cols = 3;
	blocks->color = PURPLE;
	blocks->bs[0][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[1][1] = 1;
	blocks->bs[0][2] = 1;
	break;
    case Z:
	blocks->rows = 2;
	blocks->cols = 3;
	blocks->color = RED;
	blocks->bs[0][0] = 1;
	blocks->bs[0][1] = 1;
	blocks->bs[1][1] = 1;
	blocks->bs[1][2] = 1;
	break;
    default:
	break;
    }
}

int
tetro_rotations(int block)
{
    switch(block)
    {
    case I:
    case S:
    case Z:
	return(2);
    case J:
    case L:
    case T:
	return(4);
    case O:
	return(1);
    default:
	return(0);
    }
}

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS],
	       free_blocks *blocks)
{
    int c, r;
    for (r = 0; r < blocks->rows; r++)
	for (c = 0; c < blocks->cols; c++)
	    if (blocks->bs[r][c]) // If there is a block, put it on the grid
		grid[blocks->pos.row + r][blocks->pos.col + c] = a_blocks->color;
}

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
	      free_blocks *blocks,
	      int clockwise)
{
    free_blocks *new_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    
    new_blocks->cols = blocks->rows;
    new_blocks->rows = blocks->cols;
    new_blocks->color = blocks->color;
    new_blocks->pos.row = blocks->pos.row;
    new_blocks->pos.col = blocks->pos.col + ((blocks->cols - blocks->rows) / 2);

    int r, c;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (clockwise)
		new_blocks->bs[c][new_blocks->cols - 1 - r] = a_blocks->bs[r][c];
	    else
		new_blocks->bs[new_blocks->rows - 1 - c][r] = a_blocks->bs[r][c];


    // Check for collisions
    for (r = 0; r < new_blocks->rows; r++)
	for (c = 0; c < new_blocks->cols; c++)
	    if (new_blocks->pos.row < 0 ||
		new_blocks->pos.row + r >= GRID_ROWS ||
		new_blocks->pos.col < 0 ||
		new_blocks->pos.col + c >= GRID_COLS ||
		grid[new_blocks->pos.row + r][new_blocks->pos.col + c])
		return(0);

    memcpy(blocks, new_blocks, sizeof(free_blocks));
    free(new_blocks);

    return(1);
}

int
has_lost(int grid[GRID_ROWS][GRID_COLS])
{
    int c, r;
    for (r = 0; r < 2; r++)
	for (c = 0; c < GRID_COLS; c++)
	    if (grid[r][c])
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
	    counter++; lines++;
	    // Set it to 0 in the grid copy for the animation
	    memset(new_grid[r], 0, sizeof(new_grid[r]));
	}
    }
    
    // If 1 or more row were cleared
    if (grid_changed && fpsmanager)
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

    return(counter);
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
		move_blocks(grid, a_blocks, LEFT);
		break;
	    case SDLK_RIGHT:
		move_blocks(grid, a_blocks, RIGHT);
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
	    case SDLK_a:
	    case SDLK_UP:
		rotate_blocks(grid, a_blocks, 1);
		break;
	    case SDLK_d:
		rotate_blocks(grid, a_blocks, 1);
		break;
	    case SDLK_p:
		*game_state = PAUSED;
		draw_game_paused();
		return(1);
	    case SDLK_r:
		*game_state = SPLASHSCREEN;
		return(1);
	    case SDLK_SPACE:
		// Drop the piece
		drop_blocks(grid, a_blocks);
		drop = 1;
		// This is to avoid flickerings and other nasty things
		draw_game_playing(grid, a_blocks, next_a_blocks, *score, level, lines, draw_shadow);
		break;
	    case SDLK_h:
		// Toggle the shadow
		draw_shadow = draw_shadow ? 0 : 1;
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
    
    // Level up every 10 lines
    if (lines / LINES_PER_LEVEL + 1 > level)
    {
	level++;
	fall_interval = FALL_INTERVAL - (FALL_INTERVAL / MAX_LEVEL * level);
    }

    /*
      If the user is pressing the down key, if
      enough time has passed, or if we have dropped the
      blocks, move the pieces down.
    */
    int enough_time = SDL_GetTicks() - fall_timer > fall_interval;
    if (enough_time || mov_down || drop)
    {
	/*
	  If we can move down, good, if we can't, generate new
	  active blocks.
	*/
	if (!move_blocks(grid, a_blocks, DOWN))
	{
	    // Put the blocks on the grid
	    blocks_on_grid(grid, a_blocks);

	    // Assign the score for the drop
	    *score += DROP_SCORE;

	    // Check if there are some complete rows, and update the score
	    // Calculate score
	    switch(update_grid(grid, fpsmanager))
	    {
	    case 1:
		*score += SCORE_SINGLE;
		break;
	    case 2:
		*score += SCORE_DOUBLE;
		break;
	    case 3:
		*score += SCORE_TRIPLE;
		break;
	    case 4:
		*score += SCORE_TETRIS;
		break;
	    default:
		break;
	    }

	    // Copy the blocks planned to the active blocks
	    memcpy(a_blocks, next_a_blocks, sizeof(free_blocks));

	    // Set the right column
	    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

	    // best move
	    move best_move = get_best_move(grid, a_blocks);
	    printf("Best move: %d steps %s, %d rotations.\n",
		   best_move.steps,
		   best_move.direction ? "right" : "left",
		   best_move.rotations);

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

	drop = 0; // set drop to 0 again
    }

    if (!draw_game_playing(grid, a_blocks, next_a_blocks, *score, level, lines, draw_shadow))
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

    return(1);
}

int
game_lost(int grid[GRID_ROWS][GRID_COLS],
	  GAME_STATE *game_state,
	  SDL_Event event)

{
    // Display image
    if (!draw_game_lost())
	return(0);

    //Wait
    SDL_Delay(4000);
    
    *game_state = SPLASHSCREEN;

    // Clear events
    while (SDL_PollEvent(&event))
    {
	switch (event.type)
	{
	case SDL_QUIT:
	    *game_state = QUIT;
	    break;
	}
    }

    return(1);
}
