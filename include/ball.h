#pragma once
#include <paddle.h>
#include <raylib.h>

class Ball
{
 public:
  Ball(int screenWidth, int screenHeight);

  float GetXPos();
  float GetYPos();
  float GetSpeedX();
  float GetSpeedY();

  void Draw();
  void Move(float deltaTime);
  void Reset(int screenWidth);
  void CheckCollisions(Paddle& leftPaddle, Paddle& rightPaddle, int& rightPlayerPoints,
                       int& leftPlayerPoints);

 private:
  float m_x = 0;
  float m_y = 0;
  float m_speedX = 400;
  float m_speedY = 350;
  float m_radius = 5;
  int m_screenWidth = 0;
  int m_screenHeight = 0;

  void m_CheckYCollision();
  void m_CheckXCollision(int& rightPlayerPoints, int& leftPlayerPoints);
  void m_CheckBallPaddleCollision(Paddle& leftPaddle, Paddle& rightPaddle);
};