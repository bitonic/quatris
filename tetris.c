#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_framerate.h"
#include "SDL/SDL_ttf.h"
#include "conf.h"
#include "game.h"
#include "graphics.h"

// The screen surface
SDL_Surface *screen = NULL;

// The event
SDL_Event event;

// The fps manager
FPSmanager *fpsmanager = NULL;

// The game state
GAME_STATE game_state = SPLASHSCREEN;

int grid[GRID_ROWS][GRID_COLS]; // The actual grid

// The score
int *score = 0;

int mov_down;

int
init()
{
    // Init sdl...
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
    {
	fprintf(stderr, "Unable to init SDL.\n");
	return(0);
    }

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
			      SCREEN_BPP, SDL_SWSURFACE);

    if (screen == NULL)
    {
	fprintf(stderr, "Unable to create the screen surface.\n");
	return(0);
    }

    // Init the font lib
    if (TTF_Init() == -1)
    {
	fprintf(stderr, "Unable to initialize the ttf library.\n");
	return(0);
    }

    // Init the graphics
    if (!init_graphics())
	return(0);

    // Init the game
    init_game();

    // Set the caption
    SDL_WM_SetCaption("Tetris", NULL);

    // Init the random number generator
    srand((unsigned) time(0));

    // Start the framerate manager
    fpsmanager = (FPSmanager *) malloc(sizeof(FPSmanager));
    SDL_initFramerate(fpsmanager);

    return(1);
}

void
clean_up()
{
    
    SDL_Quit();
}

int
main(int argv, char *argc[])
{
    if (!init())
	return(1);

    // Display the splash screen
    while (game_state == SPLASHSCREEN)
    {
	while (SDL_PollEvent(&event))
	{
	    switch (event.type)
	    {
	    case SDL_QUIT:
		return(0);
	    case SDL_KEYDOWN:
		game_state = PLAYING;
		break;
	    default:
		break;
	    }
	}

	SDL_framerateDelay(fpsmanager);
    }

    // Start the game
    start_game();

    while(game_state != QUIT)
    {
	switch (game_state)
	{
	case PLAYING:
	    if (!game_playing(&game_state,
			      event,
			      grid,
			      fpsmanager))
		return(1);
	    break;
	case PAUSED:
	    if (!game_paused(&game_state,
			     event))
		return(1);
	    break;
	default:
	    break;
	}

	SDL_framerateDelay(fpsmanager);
    }

    clean_up();

    return(0);
}
