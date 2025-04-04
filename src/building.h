#ifndef BUILDING_H
#define BUILDING_H


static const int EMPTY_PLACEMENT = -1;


typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST,
} Direction;

typedef enum {
    NONE,
    BELT,
    INSERTER,
    ASSEMBLY_MACHINE,
    FURNACE,
} BuildingType;


typedef struct {
    BuildingType type;
    Direction dir;
} Building;


void place_building(BuildingType type, int x, int y);
void delete_building(int x, int y);


#endif  /* BUILDING_H */
