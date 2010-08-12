#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf"
#include "SDL/SDL_framerate.h"
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

// Needed for graphics.c
SDL_Surface *blocks_sprite = NULL; // It will hold the img with the blocks
SDL_Rect *block_colors[7]; // It will hold the various rects
                                // for the various colors
int grid[GRID_ROWS][GRID_COLS]; // The actual grid

// The active blocks
free_blocks *a_blocks = NULL;

// The timer to time the falling of pieces
Uint32 fall_timer;
Uint32 fall_interval;

int mov_down;

int
init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	return(0);

    screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT,
			      SCREEN_BPP, SDL_SWSURFACE);

    if (screen == NULL)
	return(0);

    SDL_WM_SetCaption("Tetris", NULL);

    // Init the graphics
    blocks_sprite = load_image("files/blocks.png"); // Load the sprite with the blocks

    // Loads the various rects into the blocks_types array
    int i;
    for (i = 0; i < 7; i++)
    {
	block_colors[i] = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	block_colors[i]->x = i * BLOCK_SIZE;
	block_colors[i]->y = 0;
	block_colors[i]->w = BLOCK_SIZE;
	block_colors[i]->h = BLOCK_SIZE;
    }

    a_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    a_blocks->rows = 0;
    a_blocks->cols = 0;

    // The interval in wich pieces fall
    fall_interval = 700;

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
    SDL_FreeSurface(blocks_sprite);
    
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

    generate_a_blocks(a_blocks);

    // Start the timer
    fall_timer = SDL_GetTicks();

    // Set the down movement to 0
    mov_down = 0;

    while(game_state != QUIT)
    {
	switch (game_state)
	{
	case PLAYING:
	    if (!game_playing(&game_state,
			      screen,
			      blocks_sprite,
			      block_colors,
			      event,
			      grid,
			      a_blocks,
			      &fall_timer,
			      &fall_interval,
			      &mov_down,
			      fpsmanager))
		return(1);
	    break;
	case PAUSED:
	    if (!game_paused(&game_state,
			     screen,
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
