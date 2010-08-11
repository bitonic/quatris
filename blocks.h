#ifndef _blocks_h
#define _blocks_h

typedef struct {
    int col;
    int row;
} block_pos;

typedef struct {
    block_pos pos;
    int rows;
    int cols;
    int bs[4][4];
} free_blocks;

typedef enum {
    LEFT, RIGHT, UP, DOWN
} BLOCK_MOV;

typedef enum {
    I, J, L, O, S, T, Z
} BLOCK_TYPES;

#endif
