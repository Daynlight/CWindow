#pragma once
#include <string>
#include <chrono>

namespace CW::Renderer{
enum WindowMode{
  WINDOW = 0,
  BORDERLESS = 1,
  FULLSCREEN = 3
};

struct WindowData{
  bool should_close = false;
  bool vsync = 0;
  WindowMode window_mode = WindowMode::WINDOW;
  std::string title = "Window";
  bool is_focused = true;
  bool is_minimize = false;
  bool is_maximize = false;
  float delta_time = 0.0f;
  bool is_cursor_visible = true;
  bool is_cursor_on = true;
  unsigned int width;
  unsigned int height;
  unsigned int x;
  unsigned int y;
  std::string icon;
};
};

