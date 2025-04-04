#include <stdlib.h>

#include "level.h"
#include "constants.h"
#include "building.h"


Level level = {};


int initialise_level(int width, int height) {
    int cells = width * height;

    free_level();

    level.terrain = (TileType *) calloc(cells, sizeof(TileType));
    level.placement = (int *) calloc(cells, sizeof(int));
    level.buildings = (Building *) calloc(cells, sizeof(Building));

    if (
        level.terrain == NULL ||
        level.placement == NULL ||
        level.buildings == NULL
    ) {
        return 1;  // Failed to initialise
    }

    level.MAP_WIDTH = width;
    level.MAP_HEIGHT = height;
    level.MAX_CELLS = cells;
    level.CELL_WIDTH = SCREEN_WIDTH / level.MAP_WIDTH;
    level.CELL_HEIGHT = SCREEN_HEIGHT / level.MAP_HEIGHT;
    level.HALF_CELL_WIDTH = level.CELL_WIDTH / 2;
    level.HALF_CELL_HEIGHT = level.CELL_HEIGHT / 2;

    return 0;
}

void free_level(void) {
    free(level.terrain);
    free(level.placement);
    free(level.buildings);
}


int inside_level(int x, int y) {
    return x >= 0 && x < level.MAP_WIDTH && y >= 0 && y < level.MAP_HEIGHT;
}

int to_cell(int x, int y) {
    return y * level.MAP_WIDTH + x;
}
