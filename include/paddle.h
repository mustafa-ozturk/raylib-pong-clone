#pragma once
#include <raylib.h>

struct Rectangle;

class Paddle
{
 public:
  Paddle(int screenWidth, int screenHeight, int xPos);

  Rectangle GetRect() const;
  float GetYPos() const;
  float GetXPos() const;
  int GetHeight() const;

  void Draw();
  void MoveUp(float deltaTime);
  void MoveDown(float deltaTime);
  void ShrinkHeight(int amount);
  void Reset(int xPos);

 private:
  float m_x = 0;
  float m_y = 0;
  float m_speedY = 500;
  int m_width = 10;
  int m_height = 100;
  int m_screenWidth = 0;
  int m_screenHeight = 0;
};