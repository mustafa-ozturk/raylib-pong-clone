#include <ball.h>
#include <paddle.h>

#include <iostream>

#include "raylib.h"

typedef enum GameScreen
{
  START,
  GAME,
  END
} GameScreen;

void DrawStartScreen(int screenWidth, int screenHeight, bool debugMode);
void DrawPlayerPoints(int leftPlayerPoints, int rightPlayerPoints);
void UpdatePaddle(Paddle& leftPaddle, Paddle& rightPaddle, float deltaTime);
void DrawEndScreen(int rightPlayerPoints, int leftPlayerPoints, int screenWidth, int screenHeight);
void DrawDebugText(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle);
void DrawMidLine(int screenWidth, int screenHeight);

int main(int argc, char* argv[])
{
  bool debugMode = false;
  if (argc == 2 && std::string(argv[1]) == "-debug")
  {
    debugMode = true;
  }
  // init
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib pong clone");
  SetTargetFPS(60);

  GameScreen currentScreen = GameScreen::START;
  int leftPlayerPoints = 0;
  int rightPlayerPoints = 0;

  // init ball
  Ball ball(screenWidth, screenHeight);

  // init paddles
  Paddle leftPaddle(screenWidth, screenHeight, 50);
  Paddle rightPaddle(screenWidth, screenHeight, screenWidth - 50);

  // main loop
  while (!WindowShouldClose())
  {
    BeginDrawing();
    // update
    float deltaTime = GetFrameTime();
    switch (currentScreen)
    {
      case GameScreen::START:
      {
        if (IsKeyDown(KEY_SPACE))
        {
          currentScreen = GameScreen::GAME;
        }
      }
      break;
      case GameScreen::GAME:
      {
        ball.Move(deltaTime);
        ball.CheckCollisions(leftPaddle, rightPaddle, rightPlayerPoints, leftPlayerPoints);
        UpdatePaddle(leftPaddle, rightPaddle, deltaTime);

        if (rightPlayerPoints == 3 || leftPlayerPoints == 3)
        {
          currentScreen = GameScreen::END;
        }
      }
      break;
      case GameScreen::END:
      {
        if (IsKeyDown(KEY_SPACE))
        {
          ball.Reset(screenWidth);
          leftPaddle.Reset(50);
          rightPaddle.Reset(screenWidth - 50);
          leftPlayerPoints = 0;
          rightPlayerPoints = 0;
          currentScreen = GameScreen::GAME;
        }
      }
      break;
    }
    // draw
    ClearBackground(BLACK);
    switch (currentScreen)
    {
      case GameScreen::START:
      {
        DrawStartScreen(screenWidth, screenHeight, debugMode);
      }
      break;
      case GameScreen::GAME:
      {
        ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();
        DrawPlayerPoints(leftPlayerPoints, rightPlayerPoints);
        DrawMidLine(screenWidth, screenHeight);

        if (debugMode)
        {
          DrawDebugText(ball, leftPaddle, rightPaddle);
        }
      }
      break;
      case GameScreen::END:
      {
        DrawEndScreen(rightPlayerPoints, leftPlayerPoints, screenWidth, screenHeight);
      }
      break;
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void DrawStartScreen(int screenWidth, int screenHeight, bool debugMode)
{
  const char* title = "PONG";
  int titleWidth = MeasureText(title, 100);
  DrawText(title, screenWidth / 2 - titleWidth / 2, 100, 100, WHITE);

  const char* playText = "PRESS [SPACE] TO PLAY";
  int playTextWidth = MeasureText(playText, 40);

  double timeSinceInit = GetTime();
  if (int(timeSinceInit) % 2 == 0)
  {
    DrawText(playText, screenWidth / 2 - playTextWidth / 2, screenHeight / 2, 40, WHITE);
  }
  else
  {
    DrawText(playText, screenWidth / 2 - playTextWidth / 2, screenHeight / 2, 40, GRAY);
  }

  if (debugMode)
  {
    const char* debugModeText = "DEBUG MODE ENABLED";
    int debugModeTextWidth = MeasureText(debugModeText, 20);
    DrawText("DEBUG MODE ENABLED", screenWidth / 2 - debugModeTextWidth / 2, 100 + 80, 20, GREEN);
  }
}

void DrawPlayerPoints(int leftPlayerPoints, int rightPlayerPoints)
{
  int leftPlayerPointTextWidth = MeasureText(TextFormat("%i", leftPlayerPoints), 100);
  DrawText(TextFormat("%i", leftPlayerPoints),
           GetScreenWidth() / 2 - 200 - leftPlayerPointTextWidth / 2, 50, 100, WHITE);

  int rightPlayerPointTextWidth = MeasureText(TextFormat("%i", rightPlayerPoints), 100);
  DrawText(TextFormat("%i", rightPlayerPoints),
           GetScreenWidth() / 2 + 200 - rightPlayerPointTextWidth / 2, 50, 100, WHITE);
}

void UpdatePaddle(Paddle& leftPaddle, Paddle& rightPaddle, float deltaTime)
{
  // left paddle
  if (IsKeyDown(KEY_W))
  {
    leftPaddle.MoveUp(deltaTime);
  }
  if (IsKeyDown(KEY_S))
  {
    leftPaddle.MoveDown(deltaTime);
  }
  // right paddle
  if (IsKeyDown(KEY_UP))
  {
    rightPaddle.MoveUp(deltaTime);
  }
  if (IsKeyDown(KEY_DOWN))
  {
    rightPaddle.MoveDown(deltaTime);
  }
}

void DrawEndScreen(int rightPlayerPoints, int leftPlayerPoints, int screenWidth, int screenHeight)
{
  const char* winnerText = nullptr;
  if (rightPlayerPoints == 3)
  {
    winnerText = "Right Player Wins!";
  }
  else
  {
    winnerText = "Left Player Wins!";
  }
  int winnerTextWidth = MeasureText(winnerText, 60);
  DrawText(winnerText, screenWidth / 2 - winnerTextWidth / 2, 200, 60, GREEN);

  double timeSinceInit = GetTime();
  const char* replayText = "PRESS [SPACE] TO REPLAY";
  int replayTextWidth = MeasureText(replayText, 40);
  if (int(timeSinceInit) % 2 == 0)
  {
    DrawText(replayText, screenWidth / 2 - replayTextWidth / 2, screenHeight / 2, 40, WHITE);
  }
  else
  {
    DrawText(replayText, screenWidth / 2 - replayTextWidth / 2, screenHeight / 2, 40, GRAY);
  }
}

void DrawDebugText(Ball& ball, Paddle& leftPaddle, Paddle& rightPaddle)
{
  DrawFPS(0, 0);

  std::string ballPosDebug =
      "x: " + std::to_string(int(ball.GetXPos())) + " , y: " + std::to_string(int(ball.GetYPos()));
  DrawText(ballPosDebug.c_str(), int(ball.GetXPos()) + 10, int(ball.GetYPos()) + 10, 14, GREEN);

  std::string ballSpeedDebug = "ball.speed x: " + std::to_string(int(ball.GetSpeedX())) +
                               " , ball.speed y: " + std::to_string(int(ball.GetSpeedY()));
  DrawText(ballSpeedDebug.c_str(), 0, 20, 14, GREEN);


  std::cout << leftPaddle.GetYPos() << std::endl;
  DrawText(TextFormat("x: %i, y: %i", int(leftPaddle.GetXPos()), int(leftPaddle.GetYPos())), int(leftPaddle.GetXPos()) + 20, int(leftPaddle.GetYPos()), 14,
           GREEN);
  DrawText(TextFormat("x: %i, y: %i", int(rightPaddle.GetXPos()), int(rightPaddle.GetYPos())), int(rightPaddle.GetXPos()) - 100, int(rightPaddle.GetYPos()),
           14, GREEN);

  std::string getFrameTime = "GetFrameTime(): " + std::to_string(GetFrameTime());
  DrawText(getFrameTime.c_str(), 0, 40, 14, GREEN);
}

void DrawMidLine(int screenWidth, int screenHeight)
{
  for (int y = 0; y < screenHeight; y += 55)
  {
    DrawRectangle(screenWidth / 2 - 2, y, 4, 50, WHITE);
  }
}