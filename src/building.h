#ifndef BUILDING_H
#define BUILDING_H


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


void place_building(BuildingType type, int cell);
void delete_building(int cell);


#endif  /* BUILDING_H */
