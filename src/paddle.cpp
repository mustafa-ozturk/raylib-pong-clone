#include <paddle.h>

Paddle::Paddle(const int screenWidth, const int screenHeight, const int xPos)
{
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_x = xPos;
  m_y = m_screenHeight / 2;
}

float Paddle::GetYPos() const
{
  return m_y;
}

float Paddle::GetXPos() const
{
  return m_x;
}

int Paddle::GetHeight() const
{
  return m_height;
}

Rectangle Paddle::GetRect() const
{
  return Rectangle{m_x - m_width / 2,   // centerX
                   m_y - m_height / 2,  // centerY
                   static_cast<float>(m_width), static_cast<float>(m_height)};
}

void Paddle::Draw()
{
  DrawRectangleRec(GetRect(), WHITE);
}

void Paddle::MoveUp(const float deltaTime)
{
  if (m_y >= 0 + m_height / 2)
  {
    m_y -= m_speedY * deltaTime;
  }
}

void Paddle::MoveDown(const float deltaTime)
{
  if (m_y <= m_screenHeight - m_height / 2)
  {
    m_y += m_speedY * deltaTime;
  }
}

void Paddle::ShrinkHeight(const int amount)
{
  m_height -= amount;
}

void Paddle::Reset(const int xPos)
{
  m_x = xPos;
  m_y = m_screenHeight / 2;
  m_height = 100;
}