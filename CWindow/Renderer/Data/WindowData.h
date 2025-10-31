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
  bool is_cursor_visible = true;
  float delta_time = 0.0f;
  
  
  bool is_resizing = false;
  bool is_dragging = false;
  bool is_mouse_over = false; 
  bool is_keyboard_input_enabled = true;
  float opacity = 1.0f;
  unsigned int width;
  unsigned int height;
  unsigned int x;
  unsigned int y;
  std::string icon;
  float aspect_ratio;
  std::chrono::high_resolution_clock::time_point on_time;
  unsigned int monitor;
  unsigned int monitor_count;
};
};