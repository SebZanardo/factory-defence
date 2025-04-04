#include "constants.h"
#include "level.h"
#include "building.h"


// NOTE: Both these functions assume that the cell is inside the level

// TODO:
// - Add rotation
// - Add placement of different sized buildings
void place_building(BuildingType type, int cell) {
    if (level.terrain[cell] != GROUND) return;
    if (level.placement[cell] != -1) return;

    // Find first opening in list
    int i;
    for (i = 0; i < level.MAX_CELLS; i++) {
        if (level.buildings[i].type == NONE) {
            break;
        }
    }

    level.buildings[i] = (Building) { BELT, NORTH };

    level.placement[cell] = i;
}

void delete_building(int cell) {
    if (level.terrain[cell] != GROUND) return;

    int i = level.placement[cell];
    level.buildings[i].type = NONE;
    level.placement[cell] = EMPTY_PLACEMENT;
}
