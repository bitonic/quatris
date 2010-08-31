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

// MODES
// Bool that registers wheter to draw the shadow or not
int draw_shadow = 0;
// ai mode
int ai_mode = 0;
ai_move best_move;
Uint32 *ai_timer = NULL;
// super-speed mode
int super_speed = 0;
// bastard mode
int bastard_mode = 0;

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

    // Start the ai timer
    ai_timer = (Uint32 *) malloc(sizeof(Uint32));
    *ai_timer = SDL_GetTicks();
}

void
clean_up_game()
{
    free(a_blocks);
    free(next_a_blocks);
    free(ai_timer);
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

    // Reset level
    level = 0;
    
    // Generate the blocks
    generate_blocks(a_blocks, rand() % 7);
    generate_blocks(next_a_blocks, rand() % 7);

    // Set the right column
    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

    // Generate best move
    best_move = get_best_move(grid, a_blocks);

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
generate_blocks(free_blocks *blocks,
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
blocks_possible_rotations(int block)
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
		grid[blocks->pos.row + r][blocks->pos.col + c] = blocks->color;
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
    for (r = 0; r < blocks->rows; r++)
	for (c = 0; c < blocks->cols; c++)
	    if (clockwise)
		new_blocks->bs[c][new_blocks->cols - 1 - r] = blocks->bs[r][c];
	    else
		new_blocks->bs[new_blocks->rows - 1 - c][r] = blocks->bs[r][c];


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
	    counter++;
	    // Set it to 0 in the grid copy for the animation
	    memset(new_grid[r], 0, sizeof(new_grid[r]));
	}
    }
    
    // If 1 or more row were cleared
    if (grid_changed)
    {
	// Animation
	if (fpsmanager)
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

void
add_to_score(int *score, int add)
{
    if (*score + add < MAX_SCORE)
	*score += add;
    else
	*score = MAX_SCORE;
}

void
add_to_lines(int *lines, int add)
{
    if (*lines + add < MAX_LINES)
	*lines += add;
    else
	*lines = MAX_LINES;
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
	    case SDLK_p:
		*game_state = PAUSED;
		draw_game_paused();
		return(1);
	    case SDLK_r:
		*game_state = SPLASHSCREEN;
	    case SDLK_u:
		// Toggle the shadow
		draw_shadow = draw_shadow ? 0 : 1;
		break;
	    case SDLK_i:
		// toggle the AI
		ai_mode = ai_mode ? 0 : 1;
		break;
	    case SDLK_o:
		if (ai_mode)
		{
		    // Toggle the super-speed mode
		    super_speed = super_speed ? 0 : 1;
		}
		break;
	    case SDLK_b:
		// toggle bastard mode
		bastard_mode = bastard_mode ? 0 : 1;
		break;
	    default:
		break;
	    }
	    
	    // If we aren't in AI mode, enable the user's control
	    if (!ai_mode)
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
		    rotate_blocks(grid, a_blocks, 0);
		    break;
		case SDLK_SPACE:
		    // Drop the piece
		    drop_blocks(grid, a_blocks);
		    drop = 1;
		    // This is to avoid flickerings and other nasty things
//		    draw_game_playing(grid, a_blocks, next_a_blocks, *score, level, lines, draw_shadow);
		    break;
		default:
		    break;
		}

	    break;
	case SDL_KEYUP:
	    switch (event.key.keysym.sym)
	    {
	    case SDLK_DOWN:
		if (!ai_mode)
		    mov_down = 0;
		break;
	    default:
		break;
	    }
	    break;
	}
    }
    
    // Level up every 10 lines
    if (lines / LINES_PER_LEVEL + 1 > level && level < MAX_LEVEL)
    {
	level++;
	fall_interval = FALL_INTERVAL - (FALL_INTERVAL / MAX_LEVEL * level) + 80;
    }

    // If the ai_mode is on, animate
    if (ai_mode)
    {
	// If super speed is on, set mov down so that the blocks get
	// on the grid quickly
	int move_completed;
	if (super_speed)
	    move_completed = execute_ai_move(grid, a_blocks, &best_move, 0, NULL, level);
	else
	    move_completed = execute_ai_move(grid, a_blocks, &best_move, 1, ai_timer, level);

	if (move_completed)
	    mov_down = 1;
    }

    /*
      If the user is pressing the down key, if
      enough time has passed, or if we have dropped the
      blocks, move the pieces down.
    */
    int enough_time = SDL_GetTicks() - fall_timer > fall_interval;
    if (enough_time || mov_down || drop)
    {
	// If the mov_down was set by the AI, reset it
	if (ai_mode)
	    mov_down = 0;

	/*
	  If we can move down, good, if we can't, generate new
	  active blocks.
	*/
	if (!move_blocks(grid, a_blocks, DOWN))
	{
	    // Put the blocks on the grid
	    blocks_on_grid(grid, a_blocks);

	    /*
	    int priority;
	    double lolscore;
	    evaluate_grid(grid, a_blocks, &priority, &lolscore);
	    */

	    // Assign the score for the drop
	    add_to_score(score, DROP_SCORE);

	    // Check if there are some complete rows, and update the score
	    // Calculate score
	    int cleared_lines;
	    // If we are in super speed, don't animate
	    if (super_speed && ai_mode)
		cleared_lines = update_grid(grid, NULL);
	    else
		cleared_lines = update_grid(grid, fpsmanager);
	    add_to_lines(&lines, cleared_lines);
	    switch(cleared_lines)
	    {
	    case 1:
		add_to_score(score, SCORE_SINGLE);
		break;
	    case 2:
		add_to_score(score, SCORE_DOUBLE);
		break;
	    case 3:
		add_to_score(score, SCORE_DOUBLE);
		break;
	    case 4:
		add_to_score(score, SCORE_TETRIS);
		break;
	    default:
		break;
	    }

	    // If we are in bastard mode, generate the new blocks directly
	    if (bastard_mode)
		generate_blocks(a_blocks, bastard_mode_blocks(grid));
	    else
		// Copy the blocks planned to the active blocks
		memcpy(a_blocks, next_a_blocks, sizeof(free_blocks));

	    // Set the right column
	    a_blocks->pos.col = GRID_COLS / 2 - a_blocks->cols / 2;

	    // Check if the user has lost
	    if (has_lost(grid))
	    {
		// Animation
		game_over(grid, fpsmanager);
		*game_state = LOST;
		printf("Game Over. Lines Cleared: %d\n", lines);
		return(1);
	    }

	    // Generate new blocks
	    generate_blocks(next_a_blocks, rand() % 7);

	    // best move
	    best_move = get_best_move(grid, a_blocks);
	}
	if (enough_time)
	{
	    fall_timer = SDL_GetTicks(); // Reset timer
	}

	drop = 0; // set drop to 0 again
    }

    int draw_game_success;
    if (bastard_mode)
	draw_game_success = draw_game_playing(grid, a_blocks, NULL, *score, level, lines, draw_shadow);
    else
	draw_game_success = draw_game_playing(grid, a_blocks, next_a_blocks, *score, level, lines, draw_shadow);

    if (!draw_game_success)
	return(0);


    // Delay, if we aren't in super speed
    if (!super_speed || !ai_mode)
	SDL_framerateDelay(fpsmanager);

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
