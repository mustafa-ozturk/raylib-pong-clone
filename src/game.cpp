#include <game.h>

Game::Game(const int screenWidth, const int screenHeight, const bool debugMode)
    : m_leftPaddle(screenWidth, screenHeight, 50),
      m_rightPaddle(screenWidth, screenHeight, screenWidth - 50),
      m_ball(screenWidth, screenHeight)
{
  m_screenWidth = screenWidth;
  m_screenHeight = screenHeight;
  m_debugMode = debugMode;
}

void Game::Run()
{
  InitWindow(m_screenWidth, m_screenHeight, "raylib pong clone");
  SetTargetFPS(60);
  // Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
  m_redShader = LoadShader(nullptr, "../shader/red.fs");
  m_blueShader = LoadShader(nullptr, "../shader/blue.fs");
  while (!WindowShouldClose())
  {
    float deltaTime = GetFrameTime();
    Game::InputUpdate(deltaTime);
    Game::GameUpdate(deltaTime);
    Game::Draw(deltaTime);
  }
  CloseWindow();
}

void Game::InputUpdate(const float deltaTime)
{
  switch (m_currentScreen)
  {
    case GameScreen::START:
    {
      if (IsKeyDown(KEY_SPACE))
      {
        m_currentScreen = GameScreen::GAME;
      }
    }
    break;
    case GameScreen::GAME:
    {
      if (IsKeyDown(KEY_W))
      {
        m_leftPaddle.MoveUp(deltaTime);
      }
      if (IsKeyDown(KEY_S))
      {
        m_leftPaddle.MoveDown(deltaTime);
      }
      if (IsKeyDown(KEY_UP))
      {
        m_rightPaddle.MoveUp(deltaTime);
      }
      if (IsKeyDown(KEY_DOWN))
      {
        m_rightPaddle.MoveDown(deltaTime);
      }
    }
    break;
    case GameScreen::END:
    {
      if (IsKeyDown(KEY_SPACE))
      {
        m_ball.Reset(m_screenWidth);
        m_leftPaddle.Reset(50);
        m_rightPaddle.Reset(m_screenWidth - 50);
        m_leftPlayerPoints = 0;
        m_rightPlayerPoints = 0;
        m_currentScreen = GameScreen::GAME;
      }
    }
    break;
  }
}

void Game::GameUpdate(const float deltaTime)
{
  if (m_currentScreen == GameScreen::GAME)
  {
    m_ball.Move(deltaTime);
    m_ball.CheckCollisions(m_leftPaddle, m_rightPaddle, m_rightPlayerPoints, m_leftPlayerPoints);

    if (m_rightPlayerPoints == 3 || m_leftPlayerPoints == 3)
    {
      m_currentScreen = GameScreen::END;
    }
  }
}

void Game::Draw(const float deltaTime)
{
  BeginDrawing();
  ClearBackground(BLACK);
  switch (m_currentScreen)
  {
    case GameScreen::START:
    {
      Game::DrawStartScreen();
    }
    break;
    case GameScreen::GAME:
    {
      m_ball.Draw();

      BeginShaderMode(m_redShader);
      m_rightPaddle.Draw();
      EndShaderMode();
      BeginShaderMode(m_blueShader);
      m_leftPaddle.Draw();
      EndShaderMode();
      DrawPlayerPoints();
      DrawMidLine();

      if (m_debugMode)
      {
        DrawDebugText();
      }
    }
    break;
    case GameScreen::END:
    {
      DrawEndScreen();
    }
    break;
  }
  EndDrawing();
}

void Game::DrawStartScreen()
{
  const char* title = "PONG";
  int titleWidth = MeasureText(title, 100);
  DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, 100, WHITE);

  const char* playText = "PRESS [SPACE] TO PLAY";
  int playTextWidth = MeasureText(playText, 40);

  double timeSinceInit = GetTime();
  if (static_cast<int>(timeSinceInit) % 2 == 0)
  {
    DrawText(playText, m_screenWidth / 2 - playTextWidth / 2, m_screenHeight / 2, 40, WHITE);
  }
  else
  {
    DrawText(playText, m_screenWidth / 2 - playTextWidth / 2, m_screenHeight / 2, 40, GRAY);
  }

  if (m_debugMode)
  {
    const char* debugModeText = "DEBUG MODE ENABLED";
    int debugModeTextWidth = MeasureText(debugModeText, 20);
    DrawText("DEBUG MODE ENABLED", m_screenWidth / 2 - debugModeTextWidth / 2, 100 + 80, 20, GREEN);
  }
}

void Game::DrawEndScreen()
{
  const char* winnerText = nullptr;
  if (m_rightPlayerPoints == 3)
  {
    winnerText = "Right Player Wins!";
  }
  else
  {
    winnerText = "Left Player Wins!";
  }
  int winnerTextWidth = MeasureText(winnerText, 60);
  DrawText(winnerText, m_screenWidth / 2 - winnerTextWidth / 2, 200, 60, GREEN);

  double timeSinceInit = GetTime();
  const char* replayText = "PRESS [SPACE] TO REPLAY";
  int replayTextWidth = MeasureText(replayText, 40);
  if (static_cast<int>(timeSinceInit) % 2 == 0)
  {
    DrawText(replayText, m_screenWidth / 2 - replayTextWidth / 2, m_screenHeight / 2, 40, WHITE);
  }
  else
  {
    DrawText(replayText, m_screenWidth / 2 - replayTextWidth / 2, m_screenHeight / 2, 40, GRAY);
  }
}

void Game::DrawPlayerPoints()
{
  int leftPlayerPointTextWidth = MeasureText(TextFormat("%i", m_leftPlayerPoints), 100);
  DrawText(TextFormat("%i", m_leftPlayerPoints),
           m_screenWidth / 2 - 200 - leftPlayerPointTextWidth / 2, 50, 100, WHITE);

  int rightPlayerPointTextWidth = MeasureText(TextFormat("%i", m_rightPlayerPoints), 100);
  DrawText(TextFormat("%i", m_rightPlayerPoints),
           m_screenWidth / 2 + 200 - rightPlayerPointTextWidth / 2, 50, 100, WHITE);
}

void Game::DrawMidLine()
{
  for (int y = 0; y < m_screenHeight; y += 55)
  {
    DrawRectangle(m_screenWidth / 2 - 2, y, 4, 50, WHITE);
  }
}

void Game::DrawDebugText()
{
  DrawFPS(0, 0);

  std::string ballPosDebug = "x: " + std::to_string(static_cast<int>(m_ball.GetXPos())) +
                             " , y: " + std::to_string(static_cast<int>(m_ball.GetYPos()));
  DrawText(ballPosDebug.c_str(), static_cast<int>(m_ball.GetXPos()) + 10,
           static_cast<int>(m_ball.GetYPos()) + 10, 14, GREEN);

  std::string ballSpeedDebug =
      "ball.speed x: " + std::to_string(static_cast<int>(m_ball.GetSpeedX())) +
      " , ball.speed y: " + std::to_string(static_cast<int>(m_ball.GetSpeedY()));
  DrawText(ballSpeedDebug.c_str(), 0, 20, 14, GREEN);

  std::cout << m_leftPaddle.GetYPos() << std::endl;
  DrawText(TextFormat("x: %i, y: %i", static_cast<int>(m_leftPaddle.GetXPos()),
                      static_cast<int>(m_leftPaddle.GetYPos())),
           static_cast<int>(m_leftPaddle.GetXPos()) + 20, static_cast<int>(m_leftPaddle.GetYPos()),
           14, GREEN);
  DrawText(TextFormat("x: %i, y: %i", static_cast<int>(m_rightPaddle.GetXPos()),
                      static_cast<int>(m_rightPaddle.GetYPos())),
           static_cast<int>(m_rightPaddle.GetXPos()) - 100,
           static_cast<int>(m_rightPaddle.GetYPos()), 14, GREEN);

  std::string getFrameTime = "GetFrameTime(): " + std::to_string(GetFrameTime());
  DrawText(getFrameTime.c_str(), 0, 40, 14, GREEN);
}