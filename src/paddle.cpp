#include <paddle.h>

Paddle::Paddle(int screenWidth, int screenHeight, int xPos)
{
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_x = xPos;
  m_y = m_screenHeight / 2;
}

float Paddle::GetYPos()
{
  return m_y;
}

float Paddle::GetXPos()
{
  return m_x;
}

int Paddle::GetHeight()
{
  return m_height;
}

Rectangle Paddle::GetRect()
{
  return Rectangle{m_x - m_width / 2,   // centerX
                   m_y - m_height / 2,  // centerY
                   static_cast<float>(m_width), static_cast<float>(m_height)};
}

void Paddle::Draw()
{
  DrawRectangleRec(GetRect(), WHITE);
}

void Paddle::MoveUp(float deltaTime)
{
  if (m_y >= 0 + m_height / 2)
  {
    m_y -= m_speedY * deltaTime;
  }
}

void Paddle::MoveDown(float deltaTime)
{
  if (m_y <= m_screenHeight - m_height / 2)
  {
    m_y += m_speedY * deltaTime;
  }
}

void Paddle::ShrinkHeight(int amount)
{
  m_height -= amount;
}

void Paddle::Reset(int xPos)
{
  m_x = xPos;
  m_y = m_screenHeight / 2;
  m_height = 100;
}