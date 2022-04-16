#pragma once
#include <string>
#include <iostream>
namespace Raylib
{
#include <raylib.h>
}
#include <paddle.h>
#include <ball.h>


typedef enum GameScreen
{
  START,
  GAME,
  END
} GameScreen;

class Game
{
 private:
  bool m_debugMode = false;
  int m_screenWidth = 0;
  int m_screenHeight = 0;
  int m_leftPlayerPoints = 0;
  int m_rightPlayerPoints = 0;
  GameScreen m_currentScreen = GameScreen::START;
  Paddle m_leftPaddle;
  Paddle m_rightPaddle;
  Ball m_ball;


  void m_InputUpdate(float deltaTime);
  void m_GameUpdate(float deltaTime);
  void m_Draw(float deltaTime);

  void m_DrawStartScreen();
  void m_DrawPlayerPoints();
  void m_DrawEndScreen();
  void m_DrawMidLine();
  void m_DrawDebugText();

 public:
  Game(int screenWidth, int screenHeight, bool debugMode);
  void Run();
};