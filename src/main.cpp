#include "raylib.h"
#include <iostream>

typedef enum GameScreen
{
    START,
    GAME,
    END
} GameScreen;

struct Ball
{
    float x, y;
    float speedX, speedY;
    float radius;

    void Draw()
    {
        DrawCircle(int(x), int(y), radius, WHITE);
    }
};

struct Paddle
{
    float x, y;
    float speedY;
    float width, height;

    Rectangle GetRect()
    {
        //                  centerX        centerY
        return Rectangle{x - width / 2, y - height / 2, width, height};
    }

    void Draw()
    {
        DrawRectangleRec(GetRect(), WHITE);
    }
};

void DrawStartScreen();
Ball InitBall();

int main()
{
    // init
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "raylib pong clone");
    SetTargetFPS(60);

    GameScreen currentScreen = GameScreen::START;
    // init ball
    Ball ball;
    ball.x = screenWidth / 2.0f;
    ball.y = 0;
    ball.radius = 5;
    ball.speedX = 500;
    ball.speedY = 350;

    // init left paddle
    Paddle leftPaddle;
    leftPaddle.x = 50;
    leftPaddle.y = GetScreenHeight() / 2;
    leftPaddle.width = 10;
    leftPaddle.height = 100;
    leftPaddle.speedY = 500;
    // init right paddle
    Paddle rightPaddle;
    rightPaddle.x = GetScreenWidth() - 50;
    rightPaddle.y = GetScreenHeight() / 2;
    rightPaddle.width = 10;
    rightPaddle.height = 100;
    rightPaddle.speedY = 500;

    // main loop
    while (!WindowShouldClose())
    {
        BeginDrawing();
            // update
            float deltaTime = GetFrameTime();
            switch(currentScreen)
            {
                case GameScreen::GAME:
                {
                    ball.x += ball.speedX * deltaTime;
                    ball.y += ball.speedY * deltaTime;

                    if (ball.y < 0)
                    {
                        ball.speedY *= -1;
                    }
                    if (ball.y > screenHeight)
                    {
                        ball.speedY *= -1;
                    }
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
                    ball.Draw();
                    leftPaddle.Draw();
                    rightPaddle.Draw();
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