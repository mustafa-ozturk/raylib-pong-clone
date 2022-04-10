#include "raylib.h"

typedef enum GameScreen
{
    START,
    GAME,
    END
} GameScreen;

int main()
{
    // init
    const int screenWidth = 800;
    const int screenHeight = 600;
    GameScreen currentScreen = GameScreen::START;

    InitWindow(screenWidth, screenHeight, "raylib pong clone");
    SetTargetFPS(60);

    // main loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            // update
            switch(currentScreen)
            {
                case GameScreen::START:
                {

                } break;
                case GameScreen::GAME:
                {

                } break;
                case GameScreen::END:
                {

                } break;
            } 
            // draw
            ClearBackground(BLACK);
            switch(currentScreen)
            {
                case GameScreen::START:
                {
                    
                } break;
                case GameScreen::GAME:
                {

                } break;
                case GameScreen::END:
                {

                } break;
            } 
        EndDrawing();
    }

    CloseWindow();

    return 0;
}