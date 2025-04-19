#include "constants.h"
#include "level.h"
#include "building.h"
#include "update.h"
#include <stdio.h>

void find_belt(int index) {
    if (level.buildings[index].updated == true) {
        return;
    }

    int next_index = get_next_building(index, level.buildings[index].dir);

    //Next building doesn't exist
    if (next_index == -1) {
        move_belt(index,-1);
        return;
    }

    //Next building isn't a belt
    if (level.buildings[next_index].type != BELT) {
        move_belt(index,-1);
        return;
    }

    find_belt(next_index);

}

void move_belt(int index, int next_index) {
    move_items(index, next_index);
    level.buildings[index].updated = true;
    
    //Get the belt behind this.
    for (int i=0;i<4;i++) {
        int target_index = get_next_building(index,i);
        if (target_index == -1) {
            continue;
        }
        if (level.buildings[target_index].type == BELT && index == get_next_building(target_index, level.buildings[target_index].dir)) {
            move_belt(target_index,index);
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
        level.buildings[index].updated = false;
    }
    printf("tick");
}


