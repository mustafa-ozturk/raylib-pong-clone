#pragma once
#include <ball.h>
#include <paddle.h>
#include <raylib.h>

#include <iostream>
#include <string>

class Game
{
 public:
  typedef enum GameScreen
  {
    START,
    GAME,
    END
  } GameScreen;
  
  Game(const int screenWidth, const int screenHeight, const bool debugMode);
  void Run();

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
  Shader m_redShader;
  Shader m_blueShader;

  void InputUpdate(const float deltaTime);
  void GameUpdate(const float deltaTime);
  void Draw(const float deltaTime);

  void DrawStartScreen();
  void DrawPlayerPoints();
  void DrawEndScreen();
  void DrawMidLine();
  void DrawDebugText();
};