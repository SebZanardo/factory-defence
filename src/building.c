#include "constants.h"
#include "level.h"
#include "building.h"


Plan building_plan[] = {
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {1, 1}}, 4},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, -1}, {1, -1}, {-1, -1}, {-1, 1}, {-1, 0}}, 9},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}}, 5},
    {(Vector2[]){{-1, 0}, {-1, -1}, {0, 0}, {1, 0}, {1, 1}}, 5},
};


// TODO: Rotation
void place_building(BuildingType type, int cell) {
    // Check if can be placed
    int x, y;
    to_coord(cell, &x, &y);
    int ox, oy, nx, ny, new_cell;
    for (int i = 0; i < building_plan[type].size; i++) {
        ox = building_plan[type].plan[i].x;
        oy = building_plan[type].plan[i].y;

        nx = x + ox;
        ny = y + oy;
        if (!inside_level(nx, ny)) return;
        new_cell = to_cell(nx, ny);

        if (level.terrain[new_cell] != GROUND) return;
        if (level.placement[new_cell] != -1) return;
    }

    // Find first opening in list
    int index;
    for (index = 0; index < level.MAX_CELLS; index++) {
        if (level.buildings[index].type == NONE) {
            break;
        }
    }

    // Place building
    for (int i = 0; i < building_plan[type].size; i++) {
        ox = building_plan[type].plan[i].x;
        oy = building_plan[type].plan[i].y;
        new_cell = to_cell(x + ox, y + oy);

        level.placement[new_cell] = index;
    }
    level.buildings[index] = (Building) { (Vector2){x, y}, type, NORTH };
}

void delete_building(int cell) {
    int index = level.placement[cell];
    if (index == EMPTY_PLACEMENT) return;

    // Get values for building
    int x = level.buildings[index].origin.x;
    int y = level.buildings[index].origin.y;
    BuildingType type = level.buildings[index].type;

    int ox, oy;
    for (int i = 0; i < building_plan[type].size; i++) {
        ox = building_plan[type].plan[i].x;
        oy = building_plan[type].plan[i].y;
        level.placement[to_cell(x + ox, y + oy)] = EMPTY_PLACEMENT;
    }
    level.buildings[index].type = NONE;
}
