#include <iostream>

#include "raylib.h"

typedef enum GameScreen { START, GAME, END } GameScreen;

struct Ball {
  float x, y;
  float speedX, speedY;
  float radius;

  void Draw() { DrawCircle(int(x), int(y), radius, WHITE); }
};

struct Paddle {
  float x, y;
  float speedY;
  float width, height;

  Rectangle GetRect() {
    //                  centerX        centerY
    return Rectangle{x - width / 2, y - height / 2, width, height};
  }

  void Draw() { DrawRectangleRec(GetRect(), WHITE); }
};

void DrawStartScreen(int screenWidth, int screenHeight, bool debugMode);
void CheckCollision(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, int& leftPlayerPoints,
                    int& rightPlayerPoints, int screenWidth, int screenHeight);
void DrawPlayerPoints(int leftPlayerPoints, int rightPlayerPoints);
void UpdatePaddle(Paddle* leftPaddle, Paddle* rightPaddle, float deltaTime);
void DrawEndScreen(int rightPlayerPoints, int leftPlayerPoints, int screenWidth, int screenHeight);
void DrawDebugText(Ball ball, int leftPaddleX, int leftPaddleY, int rightPaddleX, int rightPaddleY);
void DrawMidLine(int screenWidth, int screenHeight);

int main(int argc, char* argv[]) {
  bool debugMode = false;
  if (argc == 2 && std::string(argv[1]) == "-debug") {
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
  const int ballBaseSpeedX = 400;
  const int ballBaseSpeedY = 350;
  Ball ball;
  ball.x = screenWidth / 2.0f;
  ball.y = 0;
  ball.radius = 5;
  ball.speedX = ballBaseSpeedX;
  ball.speedY = ballBaseSpeedY;

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
  while (!WindowShouldClose()) {
    BeginDrawing();
    // update
    float deltaTime = GetFrameTime();
    if (currentScreen == GameScreen::GAME) {
      ball.x += ball.speedX * deltaTime;
      ball.y += ball.speedY * deltaTime;
      UpdatePaddle(&leftPaddle, &rightPaddle, deltaTime);
      CheckCollision(&ball, &leftPaddle, &rightPaddle, leftPlayerPoints, rightPlayerPoints,
                     screenWidth, screenHeight);

      if (rightPlayerPoints == 3 || leftPlayerPoints == 3) {
        currentScreen = GameScreen::END;
      }
    }
    // draw
    ClearBackground(BLACK);
    switch (currentScreen) {
      case GameScreen::START: {
        DrawStartScreen(screenWidth, screenHeight, debugMode);

        if (IsKeyDown(KEY_SPACE)) {
          currentScreen = GameScreen::GAME;
        }
      } break;
      case GameScreen::GAME: {
        ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();
        DrawPlayerPoints(leftPlayerPoints, rightPlayerPoints);
        DrawMidLine(screenWidth, screenHeight);

        if (debugMode) {
          DrawDebugText(ball, leftPaddle.x, leftPaddle.y, rightPaddle.x, rightPaddle.y);
        }
      } break;
      case GameScreen::END: {
        DrawEndScreen(rightPlayerPoints, leftPlayerPoints, screenWidth, screenHeight);

        if (IsKeyDown(KEY_SPACE)) {
          ball.x = GetScreenWidth() / 2;
          ball.y = 0;
          ball.speedX = ballBaseSpeedX;
          ball.speedY = ballBaseSpeedY;
          leftPaddle.x = 50;
          leftPaddle.y = GetScreenHeight() / 2;
          leftPaddle.height = 100;
          rightPaddle.x = GetScreenWidth() - 50;
          rightPaddle.y = GetScreenHeight() / 2;
          rightPaddle.height = 100;
          leftPlayerPoints = 0;
          rightPlayerPoints = 0;
          currentScreen = GameScreen::GAME;
        }
      } break;
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void DrawStartScreen(int screenWidth, int screenHeight, bool debugMode) {
  const char* title = "PONG";
  int titleWidth = MeasureText(title, 100);
  DrawText(title, screenWidth / 2 - titleWidth / 2, 100, 100, WHITE);

  const char* playText = "PRESS [SPACE] TO PLAY";
  int playTextWidth = MeasureText(playText, 40);

  double timeSinceInit = GetTime();
  if (int(timeSinceInit) % 2 == 0) {
    DrawText(playText, screenWidth / 2 - playTextWidth / 2, screenHeight / 2, 40, WHITE);
  } else {
    DrawText(playText, screenWidth / 2 - playTextWidth / 2, screenHeight / 2, 40, GRAY);
  }

  if (debugMode) {
    const char* debugModeText = "DEBUG MODE ENABLED";
    int debugModeTextWidth = MeasureText(debugModeText, 20);
    DrawText("DEBUG MODE ENABLED", screenWidth / 2 - debugModeTextWidth / 2, 100 + 80, 20, GREEN);
  }
}

void CheckCollision(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, int& leftPlayerPoints,
                    int& rightPlayerPoints, int screenWidth, int screenHeight) {
  if (ball->y < 0) {
    ball->speedY *= -1;
  }
  if (ball->y > screenHeight) {
    ball->speedY *= -1;
  }

  if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius, leftPaddle->GetRect())) {
    if (ball->speedX < 0) {
      ball->speedX *= -1.1f;
      if (ball->y - leftPaddle->y > 0) {
        ball->speedY = 250;
      } else {
        ball->speedY = -250;
      }
      if (leftPaddle->height > 10) {
        leftPaddle->height -= 5;
        rightPaddle->height -= 5;
      }
    }
  }

  if (CheckCollisionCircleRec(Vector2{ball->x, ball->y}, ball->radius, rightPaddle->GetRect())) {
    if (ball->speedX > 0) {
      ball->speedX *= -1.1f;
      if (ball->y - rightPaddle->y > 0) {
        ball->speedY = 250;
      } else {
        ball->speedY = -250;
      }
      if (leftPaddle->height > 10) {
        leftPaddle->height -= 5;
        rightPaddle->height -= 5;
      }
    }
  }
  if (ball->x < 0) {
    leftPlayerPoints++;
    ball->x = screenWidth / 2;
    ball->y = 0;
    ball->speedX = 300;
    ball->speedY = 300;

    leftPaddle->x = 50;
    leftPaddle->y = screenHeight / 2;
    rightPaddle->x = screenWidth - 50;
    rightPaddle->y = screenHeight / 2;
  }

  if (ball->x > screenWidth) {
    rightPlayerPoints++;
    ball->x = screenWidth / 2;
    ball->y = 0;
    ball->speedX = -300;
    ball->speedY = 300;

    leftPaddle->x = 50;
    leftPaddle->y = screenHeight / 2;
    rightPaddle->x = screenWidth - 50;
    rightPaddle->y = screenHeight / 2;
  }
}

void DrawPlayerPoints(int leftPlayerPoints, int rightPlayerPoints) {
  int leftPlayerPointTextWidth = MeasureText(TextFormat("%i", leftPlayerPoints), 100);
  DrawText(TextFormat("%i", leftPlayerPoints),
           GetScreenWidth() / 2 + 200 - leftPlayerPointTextWidth / 2, 50, 100, WHITE);

  int rightPlayerPointTextWidth = MeasureText(TextFormat("%i", rightPlayerPoints), 100);
  DrawText(TextFormat("%i", rightPlayerPoints),
           GetScreenWidth() / 2 - 200 - rightPlayerPointTextWidth / 2, 50, 100, WHITE);
}

void UpdatePaddle(Paddle* leftPaddle, Paddle* rightPaddle, float deltaTime) {
  // left paddle
  if (IsKeyDown(KEY_W)) {
    if (leftPaddle->y >= 0 + leftPaddle->height / 2) {
      leftPaddle->y -= leftPaddle->speedY * deltaTime;
    }
  }
  if (IsKeyDown(KEY_S)) {
    if (leftPaddle->y <= GetScreenHeight() - leftPaddle->height / 2) {
      leftPaddle->y += leftPaddle->speedY * deltaTime;
    }
  }
  // right paddle
  if (IsKeyDown(KEY_UP)) {
    if (rightPaddle->y >= 0 + rightPaddle->height / 2) {
      rightPaddle->y -= rightPaddle->speedY * deltaTime;
    }
  }
  if (IsKeyDown(KEY_DOWN)) {
    if (rightPaddle->y <= GetScreenHeight() - rightPaddle->height / 2) {
      rightPaddle->y += rightPaddle->speedY * deltaTime;
    }
  }
}

void DrawEndScreen(int rightPlayerPoints, int leftPlayerPoints, int screenWidth, int screenHeight) {
  const char* winnerText = nullptr;
  if (rightPlayerPoints == 3 && leftPlayerPoints != 3) {
    winnerText = "Right Player Wins!";
  } else if (leftPlayerPoints == 3 && rightPlayerPoints != 3) {
    winnerText = "Left Player Wins!";
  }
  int winnerTextWidth = MeasureText(winnerText, 60);
  DrawText(winnerText, screenWidth / 2 - winnerTextWidth / 2, 200, 60, GREEN);

  double timeSinceInit = GetTime();
  const char* replayText = "PRESS [SPACE] TO REPLAY";
  int replayTextWidth = MeasureText(replayText, 40);
  if (int(timeSinceInit) % 2 == 0) {
    DrawText(replayText, screenWidth / 2 - replayTextWidth / 2, screenHeight / 2, 40, WHITE);
  } else {
    DrawText(replayText, screenWidth / 2 - replayTextWidth / 2, screenHeight / 2, 40, GRAY);
  }
}

void DrawDebugText(Ball ball, int leftPaddleX, int leftPaddleY, int rightPaddleX,
                   int rightPaddleY) {
  DrawFPS(0, 0);

  std::string ballPosDebug =
      "x: " + std::to_string(int(ball.x)) + " , y: " + std::to_string(int(ball.y));
  DrawText(ballPosDebug.c_str(), int(ball.x) + 10, int(ball.y) + 10, 14, GREEN);

  std::string ballSpeedDebug = "ball.speed x: " + std::to_string(int(ball.speedX)) +
                               " , ball.speed y: " + std::to_string(int(ball.speedY));
  DrawText(ballSpeedDebug.c_str(), 0, 20, 14, GREEN);

  DrawText(TextFormat("x: %i, y: %i", leftPaddleX, leftPaddleY), leftPaddleX + 20, leftPaddleY, 14,
           GREEN);
  DrawText(TextFormat("x: %i, y: %i", rightPaddleX, rightPaddleY), rightPaddleX - 100, rightPaddleY,
           14, GREEN);

  std::string getFrameTime = "GetFrameTime(): " + std::to_string(GetFrameTime());
  DrawText(getFrameTime.c_str(), 0, 40, 14, GREEN);
}

void DrawMidLine(int screenWidth, int screenHeight) {
  for (int y = 0; y < screenHeight; y += 55) {
    DrawRectangle(screenWidth / 2 - 2, y, 4, 50, WHITE);
  }
}