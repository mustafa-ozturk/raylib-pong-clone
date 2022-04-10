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

void DrawStartScreen();
void CheckCollision(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, int& leftPlayerPoint,
                    int& rightPlayerPoint, int screenWidth, int screenHeight);
void DrawPlayerPoints(int leftPlayerPoints, int rightPlayerPoints);
void UpdatePaddle(Paddle* leftPaddle, Paddle* rightPaddle, float deltaTime);

int main() {
  // init
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib pong clone");
  SetTargetFPS(60);

  GameScreen currentScreen = GameScreen::START;
  int leftPlayerPoints = 0;
  int rightPlayerPoints = 0;

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
  while (!WindowShouldClose()) {
    BeginDrawing();
    // update
    float deltaTime = GetFrameTime();
    switch (currentScreen) {
      case GameScreen::GAME: {
        ball.x += ball.speedX * deltaTime;
        ball.y += ball.speedY * deltaTime;
        UpdatePaddle(&leftPaddle, &rightPaddle, deltaTime);
        CheckCollision(&ball, &leftPaddle, &rightPaddle, leftPlayerPoints, rightPlayerPoints,
                       screenWidth, screenHeight);
      } break;
      case GameScreen::END: {
      } break;
    }
    // draw
    ClearBackground(BLACK);
    switch (currentScreen) {
      case GameScreen::START: {
        DrawStartScreen();

        if (IsKeyDown(KEY_SPACE)) {
          currentScreen = GameScreen::GAME;
        }
      } break;
      case GameScreen::GAME: {
        ball.Draw();
        leftPaddle.Draw();
        rightPaddle.Draw();
        DrawPlayerPoints(leftPlayerPoints, rightPlayerPoints);
      } break;
      case GameScreen::END: {
      } break;
    }
    EndDrawing();
  }

  CloseWindow();

  return 0;
}

void DrawStartScreen() {
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

void CheckCollision(Ball* ball, Paddle* leftPaddle, Paddle* rightPaddle, int& leftPlayerPoint,
                    int& rightPlayerPoint, int screenWidth, int screenHeight) {
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
    std::cout << "left player gets a point\n";
    leftPlayerPoint++;
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
    std::cout << "right player gets a point\n";
    rightPlayerPoint++;
    ball->x = screenWidth / 2;
    ball->y = 0;
    ball->speedX = 300;
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