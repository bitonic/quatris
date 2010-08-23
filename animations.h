#ifndef _animations_h
#define _animations_h

#include "SDL/SDL_framerate.h"
#include "conf.h"

void
blink_grid(int grid1[GRID_ROWS][GRID_COLS],
	   int grid2[GRID_ROWS][GRID_COLS],
	   FPSmanager *fpsmanager);

#endif
