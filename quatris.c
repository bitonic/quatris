#include <stdlib.h>
#include "SDL/SDL.h"
#include "framerate.h"
#include "SDL/SDL_ttf.h"
#include "game.h"
#include "graphics.h"

// The screen surface
SDL_Surface *screen = NULL;

// The event
SDL_Event event;

// The fps manager
fps_manager *fpsmanager = NULL;

// The game state
GAME_STATE game_state = SPLASHSCREEN;

int grid[GRID_ROWS][GRID_COLS]; // The actual grid

// The score
int score = 0;

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
  SDL_WM_SetCaption("Quatris", NULL);

  // Start the framerate manager
  fpsmanager = (fps_manager *) malloc(sizeof(fps_manager));
  init_fps_manager(fpsmanager);

  return(1);
}

int
main(int argv, char *argc[])
{
  if (!init())
    return(1);

  // Start the game
  start_game(grid);
  score = 0;

  // Display the splashscreen for 2 secs
  draw_splashscreen(0);
  // Delay
  SDL_Delay(2000);
  // Clear events
  while(SDL_PollEvent(&event))
    if (event.type == SDL_QUIT)
            return(0);

  while(game_state != QUIT)
    {
      switch (game_state)
        {
        case SPLASHSCREEN:
          while (SDL_PollEvent(&event))
            {
              switch (event.type)
                {
                case SDL_QUIT:
                  return(0);
                case SDL_KEYDOWN:
                  game_state = PLAYING;
                  start_game(grid);
                  score = 0;
                  break;
                default:
                  break;
                }
            }

          draw_splashscreen(1);

          fps_delay(fpsmanager);
          break;
        case PLAYING:
          if (!game_playing(&game_state,
                            event,
                            grid,
                            &score,
                            fpsmanager))
            return(1);
          break;
        case PAUSED:
          if (!game_paused(&game_state,
                           event))
            return(1);
          fps_delay(fpsmanager);
          break;
        case LOST:
          if (!game_lost(grid,
                         &game_state,
                         event))
            return(1);
          fps_delay(fpsmanager);
          break;
        default:
          break;
        }
    }

  clean_up_graphics();
  clean_up_game();

  SDL_Quit();

  return(0);
}
