#include <string.h>
#include "SDL/SDL_gfxPrimitives.h"
#include "graphics.h"

SDL_Color bgr_color = {0, 0, 0};

SDL_Surface *blocks_sprite = NULL; // It will hold the img with the blocks
SDL_Rect *block_colors[COLORS_QTY]; // It will hold the various rects
// for the various colors

// Images
SDL_Surface *splashscreen = NULL;
SDL_Surface *game_bgr = NULL;
SDL_Surface *paused = NULL;
SDL_Surface *pressanykey = NULL;
SDL_Surface *gameover = NULL;

// Fonts
TTF_Font *inconsolata15;
TTF_Font *inconsolata28;
SDL_Color font_color_white = {255, 255, 255};
SDL_Color font_color_red = {240, 20, 20};

void
apply_surface(SDL_Surface *source,
	      SDL_Rect *clip,
	      SDL_Surface *dest,
	      int x,
	      int y)
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    SDL_BlitSurface(source, clip, dest, &offset);
}

SDL_Surface *
load_image(char *filename)
{
    SDL_Surface *loadedImage = NULL;

    SDL_Surface *optimizedImage = NULL;

    loadedImage = IMG_Load(filename);

    if (loadedImage != NULL)
    {
	optimizedImage = SDL_DisplayFormatAlpha(loadedImage);

	SDL_FreeSurface(loadedImage);
	
    }
    else
    {
	fprintf(stderr, "Unable to open the file \"%s\".", filename);
    }
    return(optimizedImage);
}

int
init_graphics()
{
    blocks_sprite = load_image("files/blocks.png"); // Load the sprite with the blocks

    if (blocks_sprite == NULL)
	return(0);

    // Loads the various rects into the blocks_types array
    int i;
    for (i = 0; i < COLORS_QTY; i++)
    {
	block_colors[i] = (SDL_Rect *) malloc(sizeof(SDL_Rect));
	block_colors[i]->x = i * BLOCK_SIZE;
	block_colors[i]->y = 0;
	block_colors[i]->w = BLOCK_SIZE;
	block_colors[i]->h = BLOCK_SIZE;
    }

    // Splashscreen / paused screen
    splashscreen = load_image("files/splashscreen.png");
    if (splashscreen == NULL)
	return(0);

    // Main game
    game_bgr = load_image("files/game.png");
    if (game_bgr == NULL)
	return(0);

    // paused
    paused = load_image("files/paused.png");
    if (paused == NULL)
	return(0);

    pressanykey = load_image("files/pressanykey.png");
    if (pressanykey == NULL)
	return(0);

    gameover = load_image("files/gameover.png");
    if (gameover == NULL)
	return(0);

    // Init fonts

    inconsolata15 = TTF_OpenFont("files/Inconsolata.ttf", 15);
    if(!inconsolata15)
    {
	fprintf(stderr, "Error in TTF_OpenFont: %s\n", TTF_GetError());
	return(0);
    }

    inconsolata28 = TTF_OpenFont("files/Inconsolata.ttf", 28);
    if(!inconsolata28)
    {
	fprintf(stderr, "Error in TTF_OpenFont: %s\n", TTF_GetError());
	return(0);
    }
    
    return(1);
}

void
clean_up_graphics()
{
    // Frees everything

    SDL_FreeSurface(splashscreen);
    SDL_FreeSurface(game_bgr);
    SDL_FreeSurface(paused);
    SDL_FreeSurface(pressanykey);
    SDL_FreeSurface(blocks_sprite);

    TTF_CloseFont(inconsolata15);
    TTF_CloseFont(inconsolata28);
}

void
draw_block(SDL_Rect *color,
	   SDL_Surface *dest,
	   int x,
	   int y)
{
    apply_surface(blocks_sprite, color, dest, x, y);
}

void
draw_grid(int grid[GRID_ROWS][GRID_COLS],
	  SDL_Surface *dest)
{
    // Draw the background
    int grid_w = GRID_COLS * BLOCK_SIZE - 1;
    boxColor(dest, GRID_POS_X, GRID_POS_Y,
             GRID_POS_X + grid_w,
             GRID_POS_Y + 38,
             0x080808FF);
    hlineColor(dest, GRID_POS_X, GRID_POS_X + grid_w,
               GRID_POS_Y + 39, 0xBDBDBDFF);
    boxColor(dest, GRID_POS_X, GRID_POS_Y + 40,
	     GRID_POS_X + grid_w,
	     GRID_POS_Y + GRID_ROWS * BLOCK_SIZE - 1,
	     GRID_BGR);
    
    int c, r;
    for (c = 0; c < GRID_COLS; c++)
	for (r = 0; r < GRID_ROWS; r++)
	    if (grid[r][c])
		draw_block(block_colors[grid[r][c]-1], dest,
			   GRID_POS_X + c * BLOCK_SIZE,
			   GRID_POS_Y + r * BLOCK_SIZE);
}

void
draw_free_blocks(free_blocks *a_blocks,
		 SDL_Surface *dest,
		 int x, int y)
{
    int r, c;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (a_blocks->bs[r][c])
		draw_block(block_colors[a_blocks->color - 1], dest,
			   x + (a_blocks->pos.col + c) * BLOCK_SIZE,
			   y + (a_blocks->pos.row + r) * BLOCK_SIZE);
}

void
empty_grid(int grid[GRID_ROWS][GRID_COLS])
{
    int i;
    for (i = 0; i < GRID_ROWS; i++)
	memset(grid + i, 0, GRID_COLS);
}

int
draw_game_playing(int grid[GRID_ROWS][GRID_COLS],
		  free_blocks *a_blocks,
		  free_blocks *next_a_blocks,
		  int score,
		  int level,
		  int lines,
		  int draw_shadow)
{
    // Clears the screen
    SDL_FillRect(SDL_GetVideoSurface(), NULL,
		 SDL_MapRGB(SDL_GetVideoSurface()->format, 0, 0, 0));

    // Print the background
    apply_surface(game_bgr, NULL, SDL_GetVideoSurface(), 0, 0);

    
    // Display the grid
    draw_grid(grid, SDL_GetVideoSurface());
    
    if (a_blocks)
    {
	if (draw_shadow)
	    // Display the shadow
	    draw_shadow_blocks(grid, a_blocks);

	// Display active blocks
	draw_free_blocks(a_blocks, SDL_GetVideoSurface(), GRID_POS_X, GRID_POS_Y);
    }

    int x = GRID_POS_X + GRID_COLS * BLOCK_SIZE + 35;

    if (next_a_blocks)
    {
	//Display the next blocks
	draw_free_blocks(next_a_blocks, SDL_GetVideoSurface(), x, GRID_POS_Y + 25);
    }
    else
    {
    apply_surface(TTF_RenderText_Shaded(inconsolata28, "?", font_color_white, bgr_color),
		  NULL, SDL_GetVideoSurface(), x,
		  GRID_POS_Y + 25);
    }

    // Display the score
    char sscore[10];
    sprintf(sscore, "%d", score);
    apply_surface(TTF_RenderText_Shaded(inconsolata28, sscore, font_color_white, bgr_color),
		  NULL, SDL_GetVideoSurface(), x,
		  GRID_POS_Y + 108);

    // Display the level
    char slevel[3];
    sprintf(slevel, "%d", level);
    apply_surface(TTF_RenderText_Shaded(inconsolata28, slevel, font_color_white, bgr_color),
		  NULL, SDL_GetVideoSurface(), x,
		  GRID_POS_Y + 185);

    // Display the lines
    char slines[10];
    sprintf(slines, "%d", lines);
    apply_surface(TTF_RenderText_Shaded(inconsolata28, slines, font_color_white, bgr_color),
		  NULL, SDL_GetVideoSurface(), x,
		  GRID_POS_Y + 265);

    // Display the screen
    if (SDL_Flip(SDL_GetVideoSurface()) == -1)
	return(0);

    return(1);
}

void
draw_shadow_blocks(int grid[GRID_ROWS][GRID_COLS],
		   free_blocks *a_blocks)
{
    free_blocks *shadow_blocks = (free_blocks *) malloc(sizeof(free_blocks));
    memcpy(shadow_blocks, a_blocks, sizeof(free_blocks));
    shadow_blocks->color += 7;

    while (move_blocks(grid, shadow_blocks, DOWN))
	;

    draw_free_blocks(shadow_blocks, SDL_GetVideoSurface(), GRID_POS_X, GRID_POS_Y);

    free(shadow_blocks);
}

int 
draw_splashscreen(int anykey)
{
    apply_surface(splashscreen, NULL, SDL_GetVideoSurface(), 0, 0);

    if (anykey)
	apply_surface(pressanykey, NULL, SDL_GetVideoSurface(), 128, 150);
	
    if (SDL_Flip(SDL_GetVideoSurface()) == -1)
	return(0);
    return(1);
}

int
draw_game_paused()
{
    apply_surface(splashscreen, NULL, SDL_GetVideoSurface(), 0, 0);

    apply_surface(paused, NULL, SDL_GetVideoSurface(), 315, 125);

    if (SDL_Flip(SDL_GetVideoSurface()) == -1)
	return(0);
    return(1);
}

int
draw_game_lost()
{
    apply_surface(gameover, NULL, SDL_GetVideoSurface(), 0, 0);

    if (SDL_Flip(SDL_GetVideoSurface()) == -1)
	return(0);
    return(1);
}
