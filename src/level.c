#include <stdlib.h>
#include <stdio.h>

#include "level.h"
#include "constants.h"
#include "building.h"


Queue queue;
Level level;


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

int load_level(char level_path[]) {
    FILE* file_ptr = fopen(level_path, "r");

    if (file_ptr == NULL) {
        printf("ERROR: Could not open file!\n");
        return 1;
    }

    int row = 0;
    while (row < level.MAP_HEIGHT && !feof(file_ptr)) {
        int col = 0;
        while (col < level.MAP_WIDTH) {
            int value;
            int result = fscanf(file_ptr, "%d", &value);
            if (result == 1) {
                level.terrain[to_cell(col, row)] = value;
                col++;
            } else if (result == EOF) {
                break;
            } else {
                // Skip invalid input
                fscanf(file_ptr, "%*s");
            }
        }
        // Move to next line
        int ch;
        while ((ch = fgetc(file_ptr)) != '\n' && ch != EOF);
        row++;
    }

    fclose(file_ptr);

    return 0;
}

int place_path() {
    Queue_clear(&queue);

    for (int i = 0; i < level.MAX_CELLS; i++) {
        if (level.terrain[i] == PATH_OUTPUT) {
            Queue_append(&queue, i);
        }
    }

    // BFS using circular priority queue and cell index as visited map
    // Floodfill from end to start positions recording minimum number of steps
    /*int step_cells;*/
    /*int cell;*/
    /*int new_cell;*/
    /*while (1) {*/
    /*    step_cells = Queue_length(&queue);*/
    /**/
    /*    if (step_cells == 0) {*/
    /*        break;*/
    /*    }*/
    /**/
    /*    while (step_cells > 0) {*/
    /*        Queue_pop(&queue, &cell);*/
    /**/
    /*        // NESW*/
    /*        for (int d = 0; d < 4; d++) {*/
    /*            // Calculate adjacent cell position and validate*/
    /*            if (d == 0) {*/
    /*                if (cell < level.MAP_WIDTH)*/
    /*                    continue;*/
    /*                new_cell = cell - level.MAP_WIDTH;*/
    /*            } else if (d == 1) {*/
    /*                if ((cell + 1) % level.MAP_WIDTH == 0)*/
    /*                    continue;*/
    /*                new_cell = cell + 1;*/
    /*            } else if (d == 2) {*/
    /*                if (cell >= (level.MAX_CELLS - level.MAP_WIDTH))*/
    /*                    continue;*/
    /*                new_cell = cell + level.MAP_WIDTH;*/
    /*            } else if (d == 3) {*/
    /*                if (cell % level.MAP_WIDTH == 0)*/
    /*                    continue;*/
    /*                new_cell = cell - 1;*/
    /*            }*/
    /**/
    /*            // New cell hasn't been visited*/
    /*            if (level.placement[new_cell] != EMPTY_PLACEMENT) {*/
    /*                continue;*/
    /*            }*/
    /**/
    /*            // New cell isn't path tiletype*/
    /*            if (*/
    /*                level.terrain[new_cell] != PATH*/
    /*            ) {*/
    /*                continue;*/
    /*            }*/
    /**/
    /*            // Mark new_cell as visited and add to queue*/
    /*            place_building(BELT, new_cell, d);*/
    /*            Queue_append(&queue, new_cell);*/
    /*        }*/
    /*        step_cells--;*/
    /*    }*/
    /*}*/

    // TODO: If there was a start point that could not be reached then invalid

    return 0;
}

int inside_level(int x, int y) {
    return x >= 0 && x < level.MAP_WIDTH && y >= 0 && y < level.MAP_HEIGHT;
}

int to_cell(int x, int y) {
    return y * level.MAP_WIDTH + x;
}

void to_coord(int cell, int *x, int *y) {
    *y = cell / level.MAP_WIDTH;
    *x = cell % level.MAP_WIDTH;
}
