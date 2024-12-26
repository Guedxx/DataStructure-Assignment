//
// Created by nathan on 12/24/24.
//

#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "Clay/clay_renderer_raylib.c"

int main() {
    Clay_Raylib_Initialize(
        1280, 720, "Nathan && Pedro && Tuco",
        FLAG_WINDOW_RESIZABLE
    );

    const uint64_t memory_size = Clay_MinMemorySize();
    char memory[memory_size];
    Clay_Arena arena = (Clay_Arena) {
        .memory = memory,
        .capacity = memory_size
    };

    Clay_Initialize(arena, (Clay_Dimensions) {1280, 720});

    while (!WindowShouldClose()) {
        Clay_BeginLayout();



        Clay_RenderCommandArray to_render = Clay_EndLayout();

        BeginDrawing();
        Clay_Raylib_Render(to_render);
        EndDrawing();
    }
}