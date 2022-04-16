#pragma once
#include <raylib.h>

// without this struct: error: ‘Rectangle’ does not name a type
struct Rectangle; // why ? 

class Paddle
{
 private:
  float m_x = 0;
  float m_y = 0;
  float m_speedY = 500;
  int m_width = 10;
  int m_height = 100;
  int m_screenWidth = 0;
  int m_screenHeight = 0;

 public:
  Paddle(int screenWidth, int screenHeight, int xPos);

  Rectangle GetRect();
  float GetYPos();
  float GetXPos();
  int GetHeight();

  void Draw();
  void MoveUp(float deltaTime);
  void MoveDown(float deltaTime);
  void ShrinkHeight(int amount);
  void Reset(int xPos);
};