#include "SDL/SDL.h"
#include "framerate.h"
#include "graphics.h"
#include "animations.h"

void
blink_grid(int grid1[GRID_ROWS][GRID_COLS],
           int grid2[GRID_ROWS][GRID_COLS],
           fps_manager *fpsmanager)
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
      fps_delay(fpsmanager);
    }
  timer = SDL_GetTicks();
  while (SDL_GetTicks() - timer < interval)
    {
      draw_grid(grid2, SDL_GetVideoSurface());
      SDL_Flip(SDL_GetVideoSurface());
      fps_delay(fpsmanager);
    }
  timer = SDL_GetTicks();
  while (SDL_GetTicks() - timer < interval)
    {
      draw_grid(grid1, SDL_GetVideoSurface());
      SDL_Flip(SDL_GetVideoSurface());
      fps_delay(fpsmanager);
    }
}

void
game_over(int grid[GRID_ROWS][GRID_COLS],
          fps_manager *fpsmanager)
{
  int r, c;
  for (r = GRID_ROWS - 1; r >= 0; r--)
    {
      for (c = 0; c < GRID_COLS; c++)
        if (grid[r][c])
          grid[r][c] = GREY;
      draw_grid(grid, SDL_GetVideoSurface());
      SDL_Flip(SDL_GetVideoSurface());
      fps_delay(fpsmanager);
    }

  SDL_Delay(700);
}
