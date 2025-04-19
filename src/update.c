#include "constants.h"
#include "level.h"
#include "building.h"
#include "update.h"
#include <stdio.h>

void belt_update(int index) {
    if (level.buildings[index].updated == true) {
        return;
    }


    if (level.buildings[index].item_list[0] == NOTHING) {
        countdown_items(index);
        level.buildings[index].updated = true;
    }
    else {
        int next_index = get_next_building(index, level.buildings[index].dir);

        //Next building doesn't exist
        if (next_index == -1) {
            return;
        }
    
        //Next building isn't a belt or is full at last position.
        if (!(level.buildings[next_index].type == BELT && level.buildings[next_index].item_list[3] == NOTHING)) {
            return;
        }
    
        // Move first item of this building to last item of next.
        level.buildings[next_index].item_list[3] = level.buildings[index].item_list[0];
        level.buildings[index].item_list[0] = NOTHING;
        level.buildings[next_index].state = 1;
        countdown_items(index);
        level.buildings[index].updated = true;
    }

    //Since items have moved, checking the belt behind to see if it can move now.
    for (int i=0;i<4;i++) {
        int target_index = get_next_building(index,i);
        if (target_index == -1) {
            continue;
        }
        if (level.buildings[target_index].type == BELT && index == get_next_building(target_index, level.buildings[target_index].dir)) {
            belt_update(target_index);
        }
    }
}

void update(int tick) {
    for (int index = 0; index < level.MAX_CELLS; index++) {
        int variety = level.buildings[index].type;
        if (variety != NONE) {
            //Update the building by counting items down by one and applying effects.
            if (variety == BELT) {
                belt_update(index);
            }
        }
    }

    //Reset the updated boolean for all buildings
    for (int index = 0; index < level.MAX_CELLS; index++) {
        if (level.buildings[index].type == NONE) {
            continue;
        }
        if (level.buildings[index].type == BELT) {
            if (level.buildings[index].updated == false) {
                countdown_items(index);
                level.buildings[index].updated = true;
            }

            level.buildings[index].state = 0;
        }
        level.buildings[index].updated = false;
    }
}


