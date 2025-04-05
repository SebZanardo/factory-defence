#ifndef BUILDING_H
#define BUILDING_H


#include "raylib.h"


#define BUILDING_TYPES 7


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
    REFINERY,
    POWER,
} BuildingType;


typedef struct {
    Vector2 origin;
    BuildingType type;
    Direction dir;
} Building;

typedef struct {
    Vector2 *plan;
    int size;
} Plan;


extern Plan building_plan[];
static const char *building_name[] = {
    "NONE",
    "BELT",
    "INSERTER",
    "ASSEMBLY_MACHINE",
    "FURNACE",
    "REFINERY",
    "POWER",
};


void place_building(BuildingType type, int cell);
void delete_building(int cell);


#endif  /* BUILDING_H */
