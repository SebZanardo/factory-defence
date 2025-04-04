#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MAP_WIDTH 20
#define MAP_HEIGHT 9
#define EMPTY_PLACEMENT -1


// NOTE: These shouldn't be constants when we have multiple maps
const int MAX_CELLS = MAP_WIDTH * MAP_HEIGHT;
const int CELL_WIDTH = SCREEN_WIDTH / MAP_WIDTH;
const int CELL_HEIGHT = SCREEN_HEIGHT / MAP_HEIGHT;
const int HALF_CELL_WIDTH = CELL_WIDTH / 2;
const int HALF_CELL_HEIGHT = CELL_HEIGHT / 2;


typedef enum {
    EMPTY,
    GROUND,
    WATER,
    PATH,
} TileType;

typedef enum {
    NONE,
    BELT,
    INSERTER,
    ASSEMBLY_MACHINE,
    FURNACE,
} BuildingType;

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST,
} Direction;

typedef struct {
    BuildingType type;
    Direction dir;
} Building;


TileType terrain[MAP_HEIGHT][MAP_WIDTH];
int placement[MAP_HEIGHT][MAP_WIDTH];

// NOTE: We need free list eventually
Building *building_list_ptr;

void place_building(BuildingType type, int x, int y);
void delete_building(int x, int y);
int inside_map(int x, int y);


int main(void) {
    building_list_ptr = (Building *) malloc(sizeof(Building) * MAX_CELLS);

    // We need to make sure there are no garbage values in our arrays
    memset(terrain, EMPTY, sizeof(terrain));
    memset(placement, EMPTY_PLACEMENT, sizeof(placement));
    memset(building_list_ptr, 0, MAX_CELLS * sizeof(*building_list_ptr));

    // BUG: Make ground, ground. Annihilate this later when load map properly
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            terrain[y][x] = GROUND;
        }
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "factory-defence");

    while (!WindowShouldClose()) {
        Vector2 mouse_position = GetMousePosition();
        int cell_x = mouse_position.x/CELL_WIDTH;
        int cell_y = mouse_position.y/CELL_HEIGHT;
        if (IsMouseButtonPressed(0)) {
            place_building(BELT, cell_x, cell_y);
        }
        if (IsMouseButtonPressed(1)) {
            delete_building(cell_x, cell_y);
        }

        // RENDERING
        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                Rectangle rect = {
                    x * CELL_WIDTH,
                    y * CELL_HEIGHT,
                    CELL_WIDTH,
                    CELL_HEIGHT
                };
                Color colour = (x + y) % 2 == 0 ? GRAY : DARKGRAY;
                DrawRectangleRec(rect, colour);

                if (placement[y][x] != EMPTY_PLACEMENT) {
                    DrawCircle(
                        x * CELL_WIDTH + HALF_CELL_WIDTH,
                        y * CELL_HEIGHT + HALF_CELL_HEIGHT,
                        20,
                        MAGENTA
                    );
                }
            }
        }
        EndDrawing();
    }

    free(building_list_ptr);
    CloseWindow();
}


// TODO:
// - Add rotation
// - Add placement of different sized buildings
void place_building(BuildingType type, int x, int y) {
    if (!inside_map(x, y)) return;

    // TODO: Check if ground is correct type and free for all cells in building
    if (terrain[y][x] != GROUND) return;
    if (placement[y][x] != -1) return;

    // Find first opening in list
    int i;
    for (i = 0; i < MAX_CELLS; i++) {
        if (building_list_ptr[i].type == NONE) {
            break;
        }
    }

    building_list_ptr[i] = (Building) { BELT, NORTH };

    // TODO: For all cells in building
    placement[y][x] = i;
}

void delete_building(int x, int y) {
    if (!inside_map(x, y)) return;
    if (terrain[y][x] != GROUND) return;

    int i = placement[y][x];
    building_list_ptr[i].type = NONE;
    placement[y][x] = EMPTY_PLACEMENT;
}

int inside_map(int x, int y) {
    return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
}
