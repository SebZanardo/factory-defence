#include "constants.h"
#include "level.h"
#include "building.h"
#include "update.h"
#include <stdio.h>

void update(int tick) {
    for (int index = 0; index < level.MAX_CELLS; index++) {
        if (level.buildings[index].type != NONE) {
            //Update the building by counting items down by one and applying effects.
        }
    }
    printf("Tick %d", tick);
}