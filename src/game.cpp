#include <game.h>

Game::Game(int screenWidth, int screenHeight, bool debugMode)
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
  Raylib::InitWindow(m_screenWidth, m_screenHeight, "raylib pong clone");
  Raylib::SetTargetFPS(60);
  while (!Raylib::WindowShouldClose())
  {
    float deltaTime = Raylib::GetFrameTime();
    Game::m_InputUpdate(deltaTime);
    Game::m_GameUpdate(deltaTime);
    Game::m_Draw(deltaTime);
  }
  Raylib::CloseWindow();
}

void Game::m_InputUpdate(float deltaTime)
{
  switch (m_currentScreen)
  {
    case GameScreen::START:
    {
      if (Raylib::IsKeyDown(Raylib::KEY_SPACE))
      {
        m_currentScreen = GameScreen::GAME;
      }
    }
    break;
    case GameScreen::GAME:
    {
      if (Raylib::IsKeyDown(Raylib::KEY_W))
      {
        m_leftPaddle.MoveUp(deltaTime);
      }
      if (Raylib::IsKeyDown(Raylib::KEY_S))
      {
        m_leftPaddle.MoveDown(deltaTime);
      }
      if (Raylib::IsKeyDown(Raylib::KEY_UP))
      {
        m_rightPaddle.MoveUp(deltaTime);
      }
      if (Raylib::IsKeyDown(Raylib::KEY_DOWN))
      {
        m_rightPaddle.MoveDown(deltaTime);
      }
    }
    break;
    case GameScreen::END:
    {
      if (Raylib::IsKeyDown(Raylib::KEY_SPACE))
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

void Game::m_GameUpdate(float deltaTime)
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

void Game::m_Draw(float deltaTime)
{
  Raylib::BeginDrawing();
  ClearBackground(Raylib::BLACK);
  switch (m_currentScreen)
  {
    case GameScreen::START:
    {
      Game::m_DrawStartScreen();
    }
    break;
    case GameScreen::GAME:
    {
      m_ball.Draw();
      m_leftPaddle.Draw();
      m_rightPaddle.Draw();
      m_DrawPlayerPoints();
      m_DrawMidLine();

      if (m_debugMode)
      {
        m_DrawDebugText();
      }
    }
    break;
    case GameScreen::END:
    {
      m_DrawEndScreen();
    }
    break;
  }
  Raylib::EndDrawing();
}

void Game::m_DrawStartScreen()
{
  const char* title = "PONG";
  int titleWidth = Raylib::MeasureText(title, 100);
  Raylib::DrawText(title, m_screenWidth / 2 - titleWidth / 2, 100, 100, Raylib::WHITE);

  const char* playText = "PRESS [SPACE] TO PLAY";
  int playTextWidth = Raylib::MeasureText(playText, 40);

  double timeSinceInit = Raylib::GetTime();
  if (static_cast<int>(timeSinceInit) % 2 == 0)
  {
    Raylib::DrawText(playText, m_screenWidth / 2 - playTextWidth / 2, m_screenHeight / 2, 40,
                     Raylib::WHITE);
  }
  else
  {
    Raylib::DrawText(playText, m_screenWidth / 2 - playTextWidth / 2, m_screenHeight / 2, 40,
                     Raylib::GRAY);
  }

  if (m_debugMode)
  {
    const char* debugModeText = "DEBUG MODE ENABLED";
    int debugModeTextWidth = Raylib::MeasureText(debugModeText, 20);
    Raylib::DrawText("DEBUG MODE ENABLED", m_screenWidth / 2 - debugModeTextWidth / 2, 100 + 80, 20,
                     Raylib::GREEN);
  }
}

void Game::m_DrawEndScreen()
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
  int winnerTextWidth = Raylib::MeasureText(winnerText, 60);
  Raylib::DrawText(winnerText, m_screenWidth / 2 - winnerTextWidth / 2, 200, 60, Raylib::GREEN);

  double timeSinceInit = Raylib::GetTime();
  const char* replayText = "PRESS [SPACE] TO REPLAY";
  int replayTextWidth = Raylib::MeasureText(replayText, 40);
  if (static_cast<int>(timeSinceInit) % 2 == 0)
  {
    Raylib::DrawText(replayText, m_screenWidth / 2 - replayTextWidth / 2, m_screenHeight / 2, 40,
                     Raylib::WHITE);
  }
  else
  {
    Raylib::DrawText(replayText, m_screenWidth / 2 - replayTextWidth / 2, m_screenHeight / 2, 40,
                     Raylib::GRAY);
  }
}

void Game::m_DrawPlayerPoints()
{
  int leftPlayerPointTextWidth =
      Raylib::MeasureText(Raylib::TextFormat("%i", m_leftPlayerPoints), 100);
  Raylib::DrawText(Raylib::TextFormat("%i", m_leftPlayerPoints),
                   m_screenWidth / 2 - 200 - leftPlayerPointTextWidth / 2, 50, 100, Raylib::WHITE);

  int rightPlayerPointTextWidth =
      Raylib::MeasureText(Raylib::TextFormat("%i", m_rightPlayerPoints), 100);
  Raylib::DrawText(Raylib::TextFormat("%i", m_rightPlayerPoints),
                   m_screenWidth / 2 + 200 - rightPlayerPointTextWidth / 2, 50, 100, Raylib::WHITE);
}

void Game::m_DrawMidLine()
{
  for (int y = 0; y < m_screenHeight; y += 55)
  {
    Raylib::DrawRectangle(m_screenWidth / 2 - 2, y, 4, 50, Raylib::WHITE);
  }
}

void Game::m_DrawDebugText()
{
  Raylib::DrawFPS(0, 0);

  std::string ballPosDebug = "x: " + std::to_string(static_cast<int>(m_ball.GetXPos())) +
                             " , y: " + std::to_string(static_cast<int>(m_ball.GetYPos()));
  Raylib::DrawText(ballPosDebug.c_str(), static_cast<int>(m_ball.GetXPos()) + 10, static_cast<int>(m_ball.GetYPos()) + 10, 14,
                   Raylib::GREEN);

  std::string ballSpeedDebug = "ball.speed x: " + std::to_string(static_cast<int>(m_ball.GetSpeedX())) +
                               " , ball.speed y: " + std::to_string(static_cast<int>(m_ball.GetSpeedY()));
  Raylib::DrawText(ballSpeedDebug.c_str(), 0, 20, 14, Raylib::GREEN);

  std::cout << m_leftPaddle.GetYPos() << std::endl;
  Raylib::DrawText(
      Raylib::TextFormat("x: %i, y: %i", static_cast<int>(m_leftPaddle.GetXPos()), static_cast<int>(m_leftPaddle.GetYPos())),
      static_cast<int>(m_leftPaddle.GetXPos()) + 20, static_cast<int>(m_leftPaddle.GetYPos()), 14, Raylib::GREEN);
  Raylib::DrawText(Raylib::TextFormat("x: %i, y: %i", static_cast<int>(m_rightPaddle.GetXPos()),
                                      static_cast<int>(m_rightPaddle.GetYPos())),
                   static_cast<int>(m_rightPaddle.GetXPos()) - 100, static_cast<int>(m_rightPaddle.GetYPos()), 14,
                   Raylib::GREEN);

  std::string getFrameTime = "GetFrameTime(): " + std::to_string(Raylib::GetFrameTime());
  Raylib::DrawText(getFrameTime.c_str(), 0, 40, 14, Raylib::GREEN);
}