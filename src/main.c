#include <stdio.h>
#include "raylib.h"
#include "constants.h"
#include "level.h"
#include "building.h"


int main(void) {
    int selected = BELT;

    // TODO: Move map loading into level.c and from file
    if (initialise_level(20, 9) != 0) {
        printf("ERROR: Failed to initialise level\n");
        return 1;
    }
    for (int y = 0; y < level.MAP_HEIGHT; y++) {
        for (int x = 0; x < level.MAP_WIDTH; x++) {
            int cell = to_cell(x, y);
            level.terrain[cell] = GROUND;
            level.placement[cell] = EMPTY_PLACEMENT;
        }
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "factory-defence");

    while (!WindowShouldClose()) {
        // INPUT
        Vector2 mouse_position = GetMousePosition();
        float scroll = GetMouseWheelMove();

        // UPDATE
        if (scroll > 0) {
            selected++;
            if (selected >= BUILDING_TYPES) selected = 0;
        } else if (scroll < 0) {
            selected--;
            if (selected < 0) selected = BUILDING_TYPES - 1;
        }

        int cell_x = mouse_position.x/level.CELL_WIDTH;
        int cell_y = mouse_position.y/level.CELL_HEIGHT;
        if (inside_level(cell_x, cell_y)) {
            int cell = to_cell(cell_x, cell_y);
            if (IsMouseButtonDown(0)) place_building(selected, cell);
            if (IsMouseButtonDown(1)) delete_building(cell);
        }

        // RENDER
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
        DrawFPS(0, 0);
        DrawText(building_name[selected], 0, 20, 20, MAGENTA);
        EndDrawing();
    }
    CloseWindow();

    free_level();
}
