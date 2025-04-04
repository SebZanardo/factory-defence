#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#include "constants.h"
#include "level.h"
#include "building.h"


int main(void) {
    if (initialise_level(20, 9) != 0) {
        printf("Failed to initialise level\n");
        return 1;
    }

    // TODO: Move map loading into level.c and from file
    for (int y = 0; y < level.MAP_HEIGHT; y++) {
        for (int x = 0; x < level.MAP_WIDTH; x++) {
            int cell = to_cell(x, y);
            level.terrain[cell] = GROUND;
            level.placement[cell] = EMPTY_PLACEMENT;
        }
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "factory-defence");

    while (!WindowShouldClose()) {
        Vector2 mouse_position = GetMousePosition();
        int cell_x = mouse_position.x/level.CELL_WIDTH;
        int cell_y = mouse_position.y/level.CELL_HEIGHT;
        if (IsMouseButtonPressed(0)) {
            place_building(BELT, cell_x, cell_y);
        }
        if (IsMouseButtonPressed(1)) {
            delete_building(cell_x, cell_y);
        }

        // RENDERING
        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < level.MAP_HEIGHT; y++) {
            for (int x = 0; x < level.MAP_WIDTH; x++) {
                Rectangle rect = {
                    x * level.CELL_WIDTH,
                    y * level.CELL_HEIGHT,
                    level.CELL_WIDTH,
                    level.CELL_HEIGHT
                };
                Color colour = (x + y) % 2 == 0 ? GRAY : DARKGRAY;
                DrawRectangleRec(rect, colour);

                if (level.placement[to_cell(x, y)] != EMPTY_PLACEMENT) {
                    DrawCircle(
                        x * level.CELL_WIDTH + level.HALF_CELL_WIDTH,
                        y * level.CELL_HEIGHT + level.HALF_CELL_HEIGHT,
                        20,
                        MAGENTA
                    );
                }
            }
        }
        EndDrawing();
    }
    CloseWindow();
    free_level();
}
