#include <stdio.h>
#include "raylib.h"
#include "constants.h"
#include "level.h"
#include "building.h"
#include "update.h"

int main(void) {
    // Setting up the tick system
    // TODO: Make changing speed controls
    int frame_count = 0;
    int tick_count = 0;
    int tick_tps = 1;
    SetTargetFPS(MAX_FPS);

    int selected = BELT;
    Direction dir = NORTH;

    if (initialise_level(32, 16) != 0) {
        printf("ERROR: Failed to initialise level\n");
        return 1;
    }

    Queue_init(&queue, level.MAX_CELLS);
    // Clear placement map
    // TODO: Use better data types like bool and make EMPTY_PLACEMENT UINT8_MAX
    for (int i = 0; i < level.MAX_CELLS; i++) {
        level.placement[i] = EMPTY_PLACEMENT;
    }

    if (load_level("src/resources/demo.txt") != 0) {
        printf("ERROR: Failed to load level\n");
        return 1;
    }

    place_path();

    Color colour;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "factory-defence");

    while (!WindowShouldClose()) {
        // INPUT
        Vector2 mouse_position = GetMousePosition();
        float scroll = GetMouseWheelMove();

        // UPDATE building type and rotation
        if (scroll > 0) {
            selected++;
            if (selected >= BUILDING_TYPES) selected = 0;
        } else if (scroll < 0) {
            selected--;
            if (selected < 0) selected = BUILDING_TYPES - 1;
        }

        if (IsKeyPressed(KEY_R)) {
            dir ++;
            if (dir >= 4) dir = 0;
        }

        int cell_x = mouse_position.x/level.CELL_WIDTH;
        int cell_y = mouse_position.y/level.CELL_HEIGHT;
        if (inside_level(cell_x, cell_y)) {
            int cell = to_cell(cell_x, cell_y);
            if (IsMouseButtonDown(0)) place_building(selected, cell, dir);
            if (IsMouseButtonDown(1)) delete_building(cell);
            if (IsKeyDown(KEY_ONE)) {
                level.buildings[level.placement[cell]].item_list[0] = 1;  
            }
            if (IsKeyDown(KEY_TWO)) {
                level.buildings[level.placement[cell]].item_list[1] = 2;  
            }
            if (IsKeyDown(KEY_THREE)) {
                level.buildings[level.placement[cell]].item_list[2] = 3;  
            }
            if (IsKeyDown(KEY_FOUR)) {
                level.buildings[level.placement[cell]].item_list[3] = 4;  
            }
        }

        //UPDATE GAME
        if (!IsKeyDown(KEY_LEFT_SHIFT)){
            if (tick_tps > MAX_FPS) {
                for (int tick_number=0; tick_number<tick_tps/MAX_FPS; tick_number++) {
                    update(tick_count);
                    tick_count++;
                }
            }
            else {
                if (frame_count%(MAX_FPS/tick_tps)==0) {
                    update(tick_count);
                    tick_count++;
                }
            }
        }

        // RENDER
        BeginDrawing();
        ClearBackground(BLACK);
        for (int y = 0; y < level.MAP_HEIGHT; y++) {
            for (int x = 0; x < level.MAP_WIDTH; x++) {

                // Draw tile
                Rectangle rect = {
                    x * level.CELL_WIDTH,
                    y * level.CELL_HEIGHT,
                    level.CELL_WIDTH,
                    level.CELL_HEIGHT
                };

                switch (level.terrain[to_cell(x, y)]) {
                    case EMPTY:
                        colour = BLACK;
                        break;
                    case GROUND:
                        colour = (x + y) % 2 == 0 ? GRAY : DARKGRAY;
                        break;
                    case WATER:
                        colour = (Color){50,105,255,255};
                        break;
                    case PATH:
                        colour = WHITE;
                        break;
                    case PATH_INPUT:
                        colour = RED;
                        break;
                    case PATH_OUTPUT:
                        colour = GREEN;
                        break;
                }

                DrawRectangleRec(rect, colour);

                // Draw building
                if (level.placement[to_cell(x, y)] != EMPTY_PLACEMENT) {
                    DrawCircle(
                        x * level.CELL_WIDTH + level.HALF_CELL_WIDTH,
                        y * level.CELL_HEIGHT + level.HALF_CELL_HEIGHT,
                        20,
                        MAGENTA
                    );
                    int temp_x;
                    int temp_y;
                    switch (level.buildings[level.placement[to_cell(x, y)]].dir)
                    {
                    case 0:
                        temp_x = 0;
                        temp_y = -1;
                        break;
                    case 1:
                        temp_x = 1;
                        temp_y = 0;
                        break;
                    case 2:
                        temp_x = 0;
                        temp_y = 1;
                        break;
                    case 3:
                        temp_x = -1;
                        temp_y = 0;
                        break;
                    }
                    DrawCircle(
                        x * level.CELL_WIDTH + level.HALF_CELL_WIDTH*(2+temp_x)/2,
                        y * level.CELL_HEIGHT + level.HALF_CELL_HEIGHT*(2+temp_y)/2,
                        10,
                        BLUE
                    );
                    if (level.buildings[level.placement[to_cell(x, y)]].type == BELT) {
                        for (int i=0; i<4; i++) {
                            if (level.buildings[level.placement[to_cell(x, y)]].item_list[i] == NOTHING) {
                                continue;
                            }
                            Color colour_list[5] = {WHITE,DARKGRAY,MAROON,YELLOW,LIGHTGRAY};
                            DrawRectangle(
                                x * level.CELL_WIDTH + (1+2*(i%2))*(level.CELL_HEIGHT)/5,
                                y * level.CELL_HEIGHT + (1+2*(i/2))*(level.CELL_HEIGHT)/5,
                                level.CELL_WIDTH/5,
                                level.CELL_HEIGHT/5,
                                colour_list[level.buildings[level.placement[to_cell(x, y)]].item_list[i]]
                            );
                        }
                    }
                }
            }
        }
        DrawFPS(0, 0);
        DrawText(building_name[selected], 0, 20, 20, MAGENTA);
        EndDrawing();
        frame_count += 1;
    }
    CloseWindow();

    free_level();
}
