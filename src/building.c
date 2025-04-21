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




    level.buildings[index] = (Building) { (Vector2){x, y}, type, dir, {NOTHING,NOTHING,NOTHING,NOTHING}, 0, 0};

    //initalise the inputs and outputs to -1
    for (int i=0; i<MAX_BUILDING_SIZE*2+2; i++) {
        level.buildings[index].inputs[i] = -1;
        level.buildings[index].outputs[i] = -1;
    }

    // Place building and get inputs
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

        building_input(index,new_cell,dir);

        if (type==BELT) {
            belt_output(index, cell, dir);
        }
        else {
            other_output(index,cell);
        }
    }

}

//Scans directions around belt and detects valid input buildings
void building_input(int index, int cell, int dir) {
    for (int direction = 0; direction < 4; direction++) {
        int target_cell = get_new_position(cell, direction);

        //Not in map
        if (target_cell == -1) {
            continue;
        }

        //Doesn't exist
        if (level.placement[target_cell] == EMPTY_PLACEMENT) {
            continue;
        }
        
        //Same building
        if (level.placement[target_cell] == index) {
            continue;
        }

        int t_type = level.buildings[level.placement[target_cell]].type;
        int t_dir = level.buildings[level.placement[target_cell]].dir;

        if (t_type == BELT && cell == get_new_position(target_cell, t_dir)) {
            add_input(index, level.placement[target_cell]);
            add_output(level.placement[target_cell], index);
        }
        if (level.buildings[index].type == BELT && t_type != BELT && t_type != NOTHING && target_cell == get_new_position(cell, (dir+2)%4)) {
            add_input(index, level.placement[target_cell]);
            add_output(level.placement[target_cell], index);
        }
    }
}

void belt_output(int index, int cell, int dir) {
    int target_cell = get_new_position(cell, dir);

    if (target_cell == -1) {
        return;
    }

    if (level.placement[target_cell] == EMPTY_PLACEMENT) {
        return;
    }

    add_output(index, level.placement[target_cell]);
    add_input(level.placement[target_cell], index);

}

void other_output(int index, int cell) {
    for (int direction = 0; direction < 4; direction++) {
        int target_cell = get_new_position(cell, direction);

        //Not in map
        if (target_cell == -1) {
            continue;
        }

        //Doesn't exist
        if (level.placement[target_cell] == EMPTY_PLACEMENT) {
            continue;
        }
        
        //Same building
        if (level.placement[target_cell] == index) {
            continue;
        }

        int t_type = level.buildings[level.placement[target_cell]].type;
        int t_dir = level.buildings[level.placement[target_cell]].dir;

        if (t_type == BELT && cell == get_new_position(target_cell, (t_dir-2)%4)) {
            add_output(index, level.placement[target_cell]);
            add_input(level.placement[target_cell], index);
        }
    }
}

void add_output(int target_index, int output_index) {
    for (int i=0; i<MAX_BUILDING_SIZE*2+2; i++) {
        if (level.buildings[target_index].outputs[i] == -1) {
            level.buildings[target_index].outputs[i] = output_index;
            break;
        }
    }
}

void add_input(int target_index, int input_index) {
    printf("(%d -> %d)",input_index,target_index);
    for (int i=0; i<MAX_BUILDING_SIZE*2+2; i++) {
        if (level.buildings[target_index].inputs[i] == -1) {
            level.buildings[target_index].inputs[i] = input_index;
            break;
        }
    }
}

void remove_output(int target_index, int output_index) {
    bool move_up = false;
    for (int i=0; i<MAX_BUILDING_SIZE*2+2; i++) {
        if (level.buildings[target_index].outputs[i] == -1) {
            break;
        }
        if (level.buildings[target_index].outputs[i] == output_index) {
            level.buildings[target_index].outputs[i] = -1;
            move_up = true;
        }
        if (move_up == true && i<MAX_BUILDING_SIZE*2+2-1) {
            level.buildings[target_index].outputs[i] = level.buildings[target_index].outputs[i+1];
        }
    }
}

void remove_input(int target_index, int input_index) {
    bool move_up = false;
    for (int i=0; i<MAX_BUILDING_SIZE*2+2; i++) {
        if (level.buildings[target_index].inputs[i] == -1) {
            break;
        }
        if (level.buildings[target_index].inputs[i] == input_index) {
            level.buildings[target_index].inputs[i] = -1;
            move_up = true;
        }
        if (move_up == true && i<MAX_BUILDING_SIZE*2+2-1) {
            level.buildings[target_index].inputs[i] = level.buildings[target_index].inputs[i+1];
        }
    }
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

    for (int i = 0; i < MAX_BUILDING_SIZE*2+2; i++) {
        if (level.buildings[index].inputs[i] != -1) {
            remove_output(level.buildings[index].inputs[i], index);
            level.buildings[index].inputs[i] = -1;
        }
        if (level.buildings[index].outputs[i] != -1) {
            remove_input(level.buildings[index].outputs[i], index);
            level.buildings[index].outputs[i] = -1;
        }
    }
    level.buildings[index].type = NONE;
}



//NOTE: this assumes that the building is a 1x1 shape with a valid direction.
int get_new_position(int cell, Direction dir) {
    int x, y;
    to_coord(cell, &x, &y);
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
        return to_cell(x,y);
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