#include <string.h>
#include "SDL/SDL_gfxPrimitives.h"
#include "graphics.h"

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
	optimizedImage = SDL_DisplayFormat(loadedImage);

	SDL_FreeSurface(loadedImage);
    }

    return(optimizedImage);
}

void
draw_block(SDL_Rect *color,
	   SDL_Surface *dest,
	   int x,
	   int y,
	   SDL_Surface *blocks)
{
    apply_surface(blocks, color, dest, x, y);
}

void
draw_grid(int grid[GRID_ROWS][GRID_COLS],
	  SDL_Surface *dest,
	  SDL_Surface *blocks,
	  SDL_Rect *block_colors[7])
{
    // Draw the background
    boxColor(dest, GRID_POS_X, GRID_POS_Y,
	     GRID_POS_X + GRID_COLS * BLOCK_SIZE,
	     GRID_POS_Y + GRID_ROWS * BLOCK_SIZE,
	     GRID_BGR);

    int c, r;
    for (c = 0; c < GRID_COLS; c++)
	for (r = 0; r < GRID_ROWS; r++)
	    if (grid[r][c])
		draw_block(block_colors[grid[r][c]-1], dest,
			   GRID_POS_X + c * BLOCK_SIZE,
			   GRID_POS_Y + r * BLOCK_SIZE,
			   blocks);
}

void
draw_free_blocks(free_blocks *a_blocks,
		 SDL_Surface *dest,
		 int x, int y,
		 SDL_Surface *blocks,
		 SDL_Rect *block_colors[7])
{
    int r, c;
    for (r = 0; r < a_blocks->rows; r++)
	for (c = 0; c < a_blocks->cols; c++)
	    if (a_blocks->bs[r][c])
		draw_block(block_colors[a_blocks->bs[r][c] - 1], dest,
			   x + (a_blocks->pos.col + c) * BLOCK_SIZE,
			   y + (a_blocks->pos.row + r) * BLOCK_SIZE,
			   blocks);
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
		  SDL_Surface *screen,
		  SDL_Surface *blocks_sprite,
		  SDL_Rect *block_colors[7])
{
	// Display the grid
	draw_grid(grid, screen, blocks_sprite, block_colors);

	if (a_blocks)
	{
	    // Display active blocks
	    draw_free_blocks(a_blocks, screen, GRID_POS_X, GRID_POS_Y, blocks_sprite, block_colors);
	}

	if (next_a_blocks)
	{
	    int x = GRID_POS_X + GRID_COLS * BLOCK_SIZE + 50;
	    int y = GRID_POS_Y;

	    // Clear the space
	    SDL_Rect fill;
	    fill.x = x;
	    fill.y = y;
	    fill.w = 4 * BLOCK_SIZE;
	    fill.h = 2 * BLOCK_SIZE;
    
	    SDL_FillRect(screen, &fill, SDL_MapRGB(screen->format, 0, 0, 0));

	    //Display the next blocks
	    draw_free_blocks(next_a_blocks, screen,
			     GRID_POS_X + GRID_COLS * BLOCK_SIZE + 50,
			     GRID_POS_Y, blocks_sprite, block_colors);
	}

	// Display the screen
	if (SDL_Flip(screen) == -1)
	    return(0);

	return(1);
}
