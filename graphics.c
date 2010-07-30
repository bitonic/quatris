#include <string.h>
#include "graphics.h"

void
apply_surface(SDL_Surface *source, SDL_Rect *clip,
	      SDL_Surface *dest, int x, int y)
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
draw_block(SDL_Rect *color, SDL_Surface *dest, int x, int y,
	   SDL_Surface *blocks)
{
    apply_surface(blocks, color, dest, x, y);
}

void
draw_grid(int grid[GRID_ROWS][GRID_COLS], SDL_Surface *dest,
	  SDL_Surface *blocks, SDL_Rect *block_colors[7])
{
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
draw_a_blocks(free_blocks *a_blocks, SDL_Surface *dest,
	      SDL_Surface *blocks, SDL_Rect *block_colors[7])
{
    int i;
    for (i = 0; i < 4; i++)
	draw_block(block_colors[a_blocks->color - 1], dest,
		   GRID_POS_X + a_blocks->pos[i].col * BLOCK_SIZE,
		   GRID_POS_Y + a_blocks->pos[i].row * BLOCK_SIZE,
		   blocks);
}

void
empty_grid(int **grid)
{
    int i;
    for (i = 0; i < GRID_ROWS; i++)
	memset(grid + i, 0, GRID_COLS);
}
