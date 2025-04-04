#include "level.h"
#include "building.h"


// TODO:
// - Add rotation
// - Add placement of different sized buildings
void place_building(BuildingType type, int x, int y) {
    if (!inside_level(x, y)) return;

    // TODO: Check if ground is correct type and free for all cells in building
    if (level.terrain[to_cell(x, y)] != GROUND) return;
    if (level.placement[to_cell(x, y)] != -1) return;

    // Find first opening in list
    int i;
    for (i = 0; i < level.MAX_CELLS; i++) {
        if (level.buildings[i].type == NONE) {
            break;
        }
    }

    level.buildings[i] = (Building) { BELT, NORTH };

    // TODO: For all cells in building
    level.placement[to_cell(x, y)] = i;
}

void delete_building(int x, int y) {
    if (!inside_level(x, y)) return;
    if (level.terrain[to_cell(x, y)] != GROUND) return;

    int i = level.placement[to_cell(x, y)];
    level.buildings[i].type = NONE;
    level.placement[to_cell(x, y)] = EMPTY_PLACEMENT;
}
