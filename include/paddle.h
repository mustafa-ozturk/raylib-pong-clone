#pragma once
#include <raylib.h>

struct Rectangle;

class Paddle
{
 public:
  Paddle(const int screenWidth, const int screenHeight, const int xPos);

  Rectangle GetRect() const;
  float GetYPos() const;
  float GetXPos() const;
  int GetHeight() const;

  void Draw();
  void MoveUp(const float deltaTime);
  void MoveDown(const float deltaTime);
  void ShrinkHeight(const int amount);
  void Reset(const int xPos);

 private:
  float m_x = 0;
  float m_y = 0;
  float m_speedY = 500;
  int m_width = 10;
  int m_height = 100;
  int m_screenWidth = 0;
  int m_screenHeight = 0;
};