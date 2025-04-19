#include "constants.h"
#include "level.h"
#include "building.h"
#include <stdio.h>

Plan building_plan[] = {
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}}, 1},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {1, 1}}, 4},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {1, 1}, {0, -1}, {1, -1}, {-1, -1}, {-1, 1}, {-1, 0}}, 9},
    {(Vector2[]){{0, 0}, {1, 0}, {0, 1}, {-1, 0}, {0, -1}}, 5},
    {(Vector2[]){{-1, 0}, {-1, -1}, {0, 0}, {1, 0}, {1, 1}}, 5},
};

void place_building(BuildingType type, int cell, Direction dir) {

    // Check if can be placed
    int x, y;
    to_coord(cell, &x, &y);
    int ox, oy, nx, ny, new_cell;
    for (int i = 0; i < building_plan[type].size; i++) {
        ox = building_plan[type].plan[i].x;
        oy = building_plan[type].plan[i].y;

        //Rotate plan 90 degrees dir times
        int temp = 0;
        for (int j=0; j<dir; j++) {
            temp = ox;
            ox = oy;
            oy = -temp;
        }

        nx = x + ox;
        ny = y + oy;
        if (!inside_level(nx, ny)) return;
        new_cell = to_cell(nx, ny);

        if (level.terrain[new_cell] != GROUND && level.terrain[new_cell] != PATH) return;
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

        //Rotate plan 90 degrees dir times
        int temp = 0;
        for (int j=0; j<dir; j++) {
            temp = ox;
            ox = oy;
            oy = -temp;
        }

        new_cell = to_cell(x + ox, y + oy);

        level.placement[new_cell] = index;
    }
    level.buildings[index] = (Building) { (Vector2){x, y}, type, dir, {NOTHING,NOTHING,NOTHING,NOTHING}, 0, 0};
}

void delete_building(int cell) {
    int index = level.placement[cell];
    if (index == EMPTY_PLACEMENT || level.terrain[cell] == PATH) return;

    // Get values for building
    int x = level.buildings[index].origin.x;
    int y = level.buildings[index].origin.y;
    BuildingType type = level.buildings[index].type;

    int ox, oy;
    for (int i = 0; i < building_plan[type].size; i++) {
        ox = building_plan[type].plan[i].x;
        oy = building_plan[type].plan[i].y;

        int dir = level.buildings[index].dir;
        //Rotate plan 90 degrees dir times
        int temp = 0;
        for (int j=0; j<dir; j++) {
            temp = ox;
            ox = oy;
            oy = -temp;
        }

        level.placement[to_cell(x + ox, y + oy)] = EMPTY_PLACEMENT;
    }
    level.buildings[index].type = NONE;
}

//NOTE: this assumes that the building is a 1x1 shape with a valid direction.
int get_next_building(int index, Direction dir) {
    Vector2 origin = level.buildings[index].origin;
    int x = origin.x;
    int y = origin.y;

    switch (dir)
    {
    case 0:
        y--;
        break;
    case 1:
        x++;
        break;
    case 2:
        y++;
        break;
    case 3:
        x--;
        break;
    }
    if (inside_level(x,y) == true) {
        return level.placement[to_cell(x,y)];
    }
    else {
        return -1;
    }
}

//TODO: Make this applicable to different sized buildings (currently just 4)
void move_items(int index, int next_index) {

    //Moving the first item to the next building if possible
    if (next_index != -1) {
        if (level.buildings[next_index].item_list[3] == NOTHING && level.buildings[index].item_list[0] != NOTHING) {
            level.buildings[next_index].item_list[3] = level.buildings[index].item_list[0];
            level.buildings[index].item_list[0] = NOTHING;
        }
    }

    //Moving items in building.
    for (int i=1; i<4; i++) {
        if (level.buildings[index].item_list[i-1] == NOTHING && level.buildings[index].item_list[i] != NOTHING) {
            level.buildings[index].item_list[i-1] = level.buildings[index].item_list[i];
            level.buildings[index].item_list[i] = NOTHING;
        }
    }
}