#ifndef BUILDING_H
#define BUILDING_H


#include "raylib.h"
#include "constants.h"

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
    int inputs[MAX_BUILDING_SIZE*2+2];
    int outputs[MAX_BUILDING_SIZE*2+2];
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
void building_input(int index, int cell, int dir);
void belt_output(int index, int cell, int dir);
void other_output(int index, int cell);
void add_output(int target_index, int output_index);
void add_input(int target_index, int input_index);
void remove_output(int target_index, int output_index);
void remove_input(int target_index, int input_index);
void delete_building(int cell);
int get_new_position(int cell, Direction dir);
void move_items(int index, int next_index);

#endif  /* BUILDING_H */