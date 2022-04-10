#include "raylib.h"

int main()
{
    // init
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "raylib pong clone");
    SetTargetFPS(60);

    // main loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            // update
            // draw
        EndDrawing();
    }

    CloseWindow();

    return 0;
}