#include "blocks.h"
#include "graphics.h"

int
move_blocks(int grid[GRID_ROWS][GRID_COLS],
            free_blocks *blocks,
            BLOCK_MOV mov)
{
  int r, c;
  /*
    Checks that the movement doesn't confict with anything,
    specifically is not out of the grid and it doesn't go over
    another block in the grid.
  */

  switch(mov)
    {
    case LEFT:
      for (r = 0; r < blocks->rows; r++)
        for (c = 0; c < blocks->cols; c++)
          if (blocks->bs[r][c] &&           // If there is a block, check
              ((blocks->pos.col + c < 1) || // That the block is in the grid
               // And that the block at the left in the grid is empty.
               grid[blocks->pos.row + r][blocks->pos.col + c - 1]))
            return(0);
      blocks->pos.col--;
      break;
    case RIGHT:
      for (r = 0; r < blocks->rows; r++)
        for (c = 0; c < blocks->cols; c++)
          if (blocks->bs[r][c] &&
              ((blocks->pos.col + c >= GRID_COLS - 1) ||
               grid[blocks->pos.row + r][blocks->pos.col + c + 1]))
            return(0);
      blocks->pos.col++;
      break;
    case UP:
      // Don't think I'll ever use that...
      break;
    case DOWN:
      for (r = 0; r < blocks->rows; r++)
        for (c = 0; c < blocks->cols; c++)
          if (blocks->bs[r][c] &&
              ((blocks->pos.row + r >= GRID_ROWS - 1) ||
               grid[blocks->pos.row + r + 1][blocks->pos.col + c]))
            return(0);
      blocks->pos.row++;
      break;
    default:
      break;
    }
  return(1);
}

void
drop_blocks(int grid[GRID_ROWS][GRID_COLS],
            free_blocks *blocks)
{
  while (move_blocks(grid, blocks, DOWN))
    ;
}

void
generate_blocks(free_blocks *blocks,
                int new_block)
{
  // Empty the array
  memset(blocks->bs, 0, sizeof(blocks->bs));

  blocks->pos.row = 0;
  blocks->pos.col = 0;

  blocks->type = new_block;
  // Tetrominoes...
  switch(new_block)
    {
    case I:
      blocks->rows = 1;
      blocks->cols = 4;
      blocks->color = CYAN;
      blocks->bs[0][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[0][2] = 1;
      blocks->bs[0][3] = 1;
      break;
    case J:
      blocks->rows = 2;
      blocks->cols = 3;
      blocks->color = BLUE;
      blocks->bs[0][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[0][2] = 1;
      blocks->bs[1][2] = 1;
      break;
    case L:
      blocks->rows = 2;
      blocks->cols = 3;
      blocks->color = ORANGE;
      blocks->bs[0][0] = 1;
      blocks->bs[1][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[0][2] = 1;
      break;
    case O:
      blocks->rows = 2;
      blocks->cols = 2;
      blocks->color = YELLOW;
      blocks->bs[0][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[1][0] = 1;
      blocks->bs[1][1] = 1;
      break;
    case S:
      blocks->rows = 2;
      blocks->cols = 3;
      blocks->color = GREEN;
      blocks->bs[1][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[1][1] = 1;
      blocks->bs[0][2] = 1;
      break;
    case T:
      blocks->rows = 2;
      blocks->cols = 3;
      blocks->color = PURPLE;
      blocks->bs[0][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[1][1] = 1;
      blocks->bs[0][2] = 1;
      break;
    case Z:
      blocks->rows = 2;
      blocks->cols = 3;
      blocks->color = RED;
      blocks->bs[0][0] = 1;
      blocks->bs[0][1] = 1;
      blocks->bs[1][1] = 1;
      blocks->bs[1][2] = 1;
      break;
    default:
      break;
    }
}

int
blocks_possible_rotations(int block)
{
  switch(block)
    {
    case I:
    case S:
    case Z:
      return(2);
    case J:
    case L:
    case T:
      return(4);
    case O:
      return(1);
    default:
      return(0);
    }
}

int
rotate_blocks(int grid[GRID_ROWS][GRID_COLS],
              free_blocks *blocks,
              int clockwise)
{
  free_blocks *new_blocks = (free_blocks *) malloc(sizeof(free_blocks));

  new_blocks->cols = blocks->rows;
  new_blocks->rows = blocks->cols;
  new_blocks->color = blocks->color;
  new_blocks->pos.row = blocks->pos.row;
  new_blocks->pos.col = blocks->pos.col + ((blocks->cols - blocks->rows) / 2);

  int r, c;
  for (r = 0; r < blocks->rows; r++)
    for (c = 0; c < blocks->cols; c++)
            if (clockwise)
        new_blocks->bs[c][new_blocks->cols - 1 - r] = blocks->bs[r][c];
            else
        new_blocks->bs[new_blocks->rows - 1 - c][r] = blocks->bs[r][c];


  // Check for collisions
  for (r = 0; r < new_blocks->rows; r++)
    for (c = 0; c < new_blocks->cols; c++)
            if (new_blocks->pos.row < 0 ||
          new_blocks->pos.row + r >= GRID_ROWS ||
          new_blocks->pos.col < 0 ||
          new_blocks->pos.col + c >= GRID_COLS ||
          grid[new_blocks->pos.row + r][new_blocks->pos.col + c])
        return(0);

  memcpy(blocks, new_blocks, sizeof(free_blocks));
  free(new_blocks);

  return(1);
}

void
blocks_on_grid(int grid[GRID_ROWS][GRID_COLS],
               free_blocks *blocks)
{
  int c, r;
  for (r = 0; r < blocks->rows; r++)
    for (c = 0; c < blocks->cols; c++)
            if (blocks->bs[r][c]) // If there is a block, put it on the grid
        grid[blocks->pos.row + r][blocks->pos.col + c] = blocks->color;
}
