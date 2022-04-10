#include "raylib.h"

typedef enum GameScreen
{
    START,
    GAME,
    END
} GameScreen;

void DrawStartScreen();

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
                    DrawStartScreen();

                    if (IsKeyDown(KEY_SPACE))
                    {
                        currentScreen = GameScreen::GAME;
                    }
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

void DrawStartScreen()
{
    const char* title = "PONG";
    int titleWidth = MeasureText(title, 100);
    DrawText(title, GetScreenWidth() / 2 - titleWidth / 2, 100, 100, WHITE);

    const char* playText = "PRESS [SPACE] TO PLAY";
    int playTextWidth = MeasureText(playText, 40);

    double timeSinceInit = GetTime();
    if (int(timeSinceInit) % 2 == 0) {
        DrawText(playText, GetScreenWidth() / 2 - playTextWidth / 2, GetScreenHeight() / 2, 40, WHITE);
    } else {
        DrawText(playText, GetScreenWidth() / 2 - playTextWidth / 2, GetScreenHeight() / 2, 40, GRAY);
    }
}