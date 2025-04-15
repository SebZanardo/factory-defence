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
    level.buildings[index] = (Building) { (Vector2){x, y}, type, dir};
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

//TODO: These functions will use the space of the itemlist in building but 8 is the standin 
int get_number_of_nothings(Building building) {
    int amount = 0;
    for (int i=0; i<8; i++) {
        if (building.item_list[i].type == NOTHING) {
            amount += 1;
        }
    }
    return amount;
}

void countdown_items(Building building) {
    for (int i=0; i<8; i++) {
        if (building.item_list[i].type != NOTHING && building.item_list[i].counter > 0) {
            building.item_list[i].counter -= 1;
        }
    }
}

void add_item(Building building, Item item) {
    for (int i=0; i<8; i++) {
        if (building.item_list[i].type == NOTHING) {
            building.item_list[i] = item;
            break;
        }
    }
}

