#ifndef _framerate_h
#define _framerate_h

#include "SDL/SDL.h"

typedef struct {
    int fps;
    Uint32 frame_interval;
    Uint32 last_ticks;
} fps_manager;

void
init_fps_manager(fps_manager *fpsmanager);

void
fps_delay(fps_manager *);

#endif
