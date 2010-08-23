#include "SDL/SDL.h"
#include "SDL/SDL_framerate.h"
#include "graphics.h"

void
blink_grid(int grid1[GRID_ROWS][GRID_COLS],
	   int grid2[GRID_ROWS][GRID_COLS],
	   FPSmanager *fpsmanager)
{
    Uint32 timer, interval;

    // Animate
    timer = SDL_GetTicks();
    interval = BLINK_INTERVAL; // Interval between the blinking

    // Displays intermittedly the old grid and the
    // grid with the rows filled with 0s
    while (SDL_GetTicks() - timer < interval)
    {
	draw_grid(grid1, SDL_GetVideoSurface());
	SDL_Flip(SDL_GetVideoSurface());
	SDL_framerateDelay(fpsmanager);
    }
    timer = SDL_GetTicks();
    while (SDL_GetTicks() - timer < interval)
    {
	draw_grid(grid2, SDL_GetVideoSurface());
	SDL_Flip(SDL_GetVideoSurface());
	SDL_framerateDelay(fpsmanager);
    }
    timer = SDL_GetTicks();
    while (SDL_GetTicks() - timer < interval)
    {
	draw_grid(grid1, SDL_GetVideoSurface());
	SDL_Flip(SDL_GetVideoSurface());
	SDL_framerateDelay(fpsmanager);
    }
}
