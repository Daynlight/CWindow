#pragma once

namespace CW::Renderer{
enum WindowMode{
  WINDOW = 0,
  BOARDLESS = 1,
  FULLSCREEN = 3
};

struct WindowData{
  unsigned int width;
  unsigned int height;
  unsigned int x;
  unsigned int y;
  bool should_close = true;
  bool vsync = 0;
  WindowMode window_mode = WindowMode::WINDOW;
};
}