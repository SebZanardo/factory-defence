#ifndef LEVEL_H
#define LEVEL_H


#include "building.h"


typedef enum {
    EMPTY,
    GROUND,
    WATER,
    PATH,
} TileType;

typedef struct {
    TileType *terrain;
    int *placement;
    Building *buildings;
    int MAP_WIDTH;
    int MAP_HEIGHT;
    int MAX_CELLS;
    int CELL_WIDTH;
    int CELL_HEIGHT;
    int HALF_CELL_WIDTH;
    int HALF_CELL_HEIGHT;
} Level;


extern Level level;


int initialise_level(int width, int height);
void free_level(void);
int inside_level(int x, int y);
int to_cell(int x, int y);
void to_coord(int cell, int *x, int *y);


#endif  /* LEVEL_H */
