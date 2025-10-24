#pragma once
#include <string>
#include <chrono>
#include <unordered_map>

namespace CW::Renderer{
struct InputData{
  double mouse_x;
  double mouse_y;
  double scroll_x;
  double scroll_y;
  bool scroll_is_down;
  bool left_mouse_button_is_down;
  bool right_mouse_button_is_down;

  std::unordered_map<char, bool> keys_down;
  bool is_key_down(char key) const {
    auto it = keys_down.find(key);
    return it != keys_down.end() && it->second;
  };
};
};