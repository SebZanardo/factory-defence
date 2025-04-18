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

typedef enum {
    NOTHING,
    COPPER_ORE,
    IRON_ORE,
    COPPER,
    IRON,
} Item;

typedef struct {
    Vector2 origin;
    BuildingType type;
    Direction dir;
    Item item_list[4];
    bool updated;
    int state;
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


void place_building(BuildingType type, int cell, Direction dir);
void delete_building(int cell);
int get_next_building(int index, Direction dir);
void countdown_items(int index);

#endif  /* BUILDING_H */
