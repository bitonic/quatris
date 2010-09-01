#ifndef _conf_h
#define _conf_h

/*
  GAME
*/
// Defines..
#define GRID_COLS 10
#define GRID_ROWS 22
#define GRID_ACTUAL_ROWS 20

#define DROP_SCORE 6
#define SCORE_SINGLE 40
#define SCORE_DOUBLE 100
#define SCORE_TRIPLE 300
#define SCORE_TETRIS 1200
#define MAX_SCORE 999999999

#define FALL_INTERVAL 600

#define LINES_PER_LEVEL 10
#define MAX_LEVEL 15
#define MAX_LINES 999999999

/*
  GRAPHICS
*/
#define SCREEN_WIDTH 530
#define SCREEN_HEIGHT 480
#define SCREEN_BPP 32

// The length of the block side
#define BLOCK_SIZE 20
// The position of the grid
#define GRID_POS_X 20
#define GRID_POS_Y 20

// Colors
#define GRID_BGR 0x101010FF
#define GRID_BGR_UP 0x080808FF
#define GRID_LINE 0xBDBDBDFF

/*
  ANIMATIONS
*/
#define BLINK_INTERVAL 100

/*
  AI
*/
#define AI_ANIMATION_INTERVAL 200

#endif
