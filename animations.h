#ifndef _animations_h
#define _animations_h

#include "framerate.h"

void
blink_grid(int grid1[GRID_ROWS][GRID_COLS],
           int grid2[GRID_ROWS][GRID_COLS],
           fps_manager *fpsmanager);

void
game_over(int[GRID_ROWS][GRID_COLS],
          fps_manager *fpsmanager);

#endif
