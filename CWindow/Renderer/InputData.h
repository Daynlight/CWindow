#pragma once
#include <string>
#include <chrono>

namespace CW::Renderer{
struct InputData{
  double mouse_x;
  double mouse_y;
  double scroll_x;
  double scroll_y;
  bool scroll_is_down;
  bool left_mouse_button_is_down;
  bool right_mouse_button_is_down;
};
};