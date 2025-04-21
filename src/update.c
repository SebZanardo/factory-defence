#include "constants.h"
#include "level.h"
#include "building.h"
#include "update.h"
#include <stdio.h>

void find_belt(int index) {
    if (level.buildings[index].updated == true) {
        return;
    }

    int next_index = level.buildings[index].outputs[0];

    //Next building doesn't exist
    if (next_index == -1) {
        move_belt(index);
        return;
    }

    //Next building isn't a belt
    if (level.buildings[next_index].type != BELT) {
        move_belt(index);
        return;
    }

    //Loop detection
    if (level.buildings[next_index].state == 2) {
        move_belt(index);
    }

    if (level.buildings[next_index].state == 1) {
        for(int i=0;i<4;i++) {
            if (level.buildings[index].item_list[i] == NOTHING) {
                move_belt(index);
            }
        }
    }

    level.buildings[index].state++;
    find_belt(next_index);

}

void move_belt(int index) {
    if (level.buildings[index].updated == true) {
        return;
    }
    move_items(index, level.buildings[index].outputs[0]);
    level.buildings[index].updated = true;
    
    //Get the belt behind this.
    for (int i=0;i<MAX_BUILDING_SIZE*2+2;i++) {
        int target = level.buildings[index].inputs[i];
        if (target == -1) {
            break;
        }
        if (level.buildings[target].type == BELT) {
            move_belt(target);
        }
    }
}

void update(int tick) {
    for (int index = 0; index < level.MAX_CELLS; index++) {
        int variety = level.buildings[index].type;
        if (variety != NONE) {
            //Update specific building types
            if (variety == BELT) {
                find_belt(index);
            }
        }
    }

    //Reset the updated boolean for all buildings
    for (int index = 0; index < level.MAX_CELLS; index++) {
        if (level.buildings[index].type == NONE) {
            continue;
        }
        if (level.buildings[index].type == BELT) {
            level.buildings[index].state = 0;
        }
        level.buildings[index].updated = false;
    }
}