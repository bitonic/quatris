#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
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

// Needed for graphics.c
SDL_Surface *blocks_sprite = NULL; // It will hold the img with the blocks
SDL_Rect *block_colors[COLORS]; // It will hold the various rects
                                // for the various colors
int grid[GRID_ROWS][GRID_COLS]; // The actual grid

// The active blocks
active_blocks *a_blocks = NULL;

// The movement requested by the user
int mov_left, mov_right, mov_down;

// The timer to time the falling of pieces
Uint32 fall_timer;
Uint32 fall_interval;

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
    for (i = 0; i < COLORS; i++)
    {
	block_colors[i] = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	block_colors[i]->x = i * BLOCK_SIZE;
	block_colors[i]->y = 0;
	block_colors[i]->w = BLOCK_SIZE;
	block_colors[i]->h = BLOCK_SIZE;
    }

    a_blocks = (active_blocks *) malloc(sizeof(active_blocks));

    // The interval in wich pieces fall
    fall_interval = 1000;

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
    int quit = 0;

    if (!init())
	return(1);

    set_active_blocks(0, 3,
		      0, 4,
		      1, 4,
		      1, 5,
		      RED,
		      a_blocks);

    // Start the timer
    fall_timer = SDL_GetTicks();

    while(!quit)
    {
	while (SDL_PollEvent(&event))
	{
	    switch (event.type)
	    {
	    case SDL_QUIT:
		quit = 1;
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
		    mov_down = 1;
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

	if (SDL_Flip(screen) == -1)
	    return(1);
 
	if (mov_down)
	    move_blocks(grid, a_blocks, DOWN);
	else if (SDL_GetTicks() - fall_timer > fall_interval)
	{
	    if (!move_blocks(grid, a_blocks, DOWN))
	    {
		blocks_on_grid(grid, a_blocks);
		generate_a_blocks(a_blocks);
	    }
	    fall_timer = SDL_GetTicks();
	}

	// Clears the screen
        SDL_FillRect(screen, &screen->clip_rect, SDL_MapRGB(screen->format, 0, 0, 0));

	// Display the grid
	draw_grid(grid, screen, blocks_sprite, block_colors);

	// Display active blocks
	draw_a_blocks(a_blocks, screen, blocks_sprite, block_colors);

	// Delay
	SDL_framerateDelay(fpsmanager);

    }

    clean_up();

    return(0);
}
