#ifndef _game_h
#define _game_h

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_framerate.h"
#include "SDL/SDL_ttf.h"
#include "blocks.h"
#include "conf.h"

// The game state
typedef enum {
    PLAYING, PAUSED, LOST, SPLASHSCREEN, QUIT
} GAME_STATE;

void
init_game();

void
start_game(int grid[GRID_ROWS][GRID_COLS]);

void
clean_up_game();

/*
  HELPERS
*/

// Checks if the player has lost
int
has_lost(int grid[GRID_ROWS][GRID_COLS]);

/*
  If there are complete rows, then it animates
  the grid and removes the rows from it.
  Returns the number of completed rows
*/
int
update_grid(int grid[GRID_ROWS][GRID_COLS],
	    FPSmanager *fpsmanager);


/*
  GAME STATES
*/

int
game_playing(GAME_STATE *game_state,
	     SDL_Event event,
	     int grid[GRID_ROWS][GRID_COLS],
	     int *score,
	     FPSmanager *fpsmanager);

int
game_paused(GAME_STATE *game_state,
	    SDL_Event event);

int
game_lost(int grid[GRID_ROWS][GRID_COLS],
	  GAME_STATE *game_state,
	  SDL_Event event);

#endif
