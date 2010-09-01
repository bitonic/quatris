#include "framerate.h"
#include "conf.h"

void
init_fps_manager(fps_manager *fpsmanager)
{
    fpsmanager->fps = DEFAULT_FRAMERATE;

    fpsmanager->frame_interval = 1000 / DEFAULT_FRAMERATE;
    fpsmanager->last_ticks = SDL_GetTicks();
}

void
fps_delay(fps_manager *fpsmanager)
{
    Uint32 difference = SDL_GetTicks() - fpsmanager->last_ticks;

    if (difference < fpsmanager->frame_interval)
        SDL_Delay(fpsmanager->frame_interval - difference);

    fpsmanager->last_ticks = SDL_GetTicks();
}
