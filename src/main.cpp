#include <iostream>
#include "raylib.h"
#include <game.h>

int main(int argc, char* argv[])
{
  bool debugMode = false;
  if (argc == 2 && std::string(argv[1]) == "-debug")
  {
    debugMode = true;
  }
  const int screenWidth = 800;
  const int screenHeight = 600;
  Game game(screenWidth, screenHeight, debugMode);
  game.Run();

  return 0;
}
