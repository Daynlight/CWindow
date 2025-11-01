#pragma once
#include <string>
#include <chrono>
#include <unordered_map>

namespace CW::Renderer{
struct InputData{
  int mouse_x;
  int mouse_y;

  int mouse_scroll_x;
  int mouse_scroll_y;

  bool scroll_is_down;
  bool left_mouse_button_is_down;
  bool right_mouse_button_is_down;

  bool back_mouse_button_is_down;
  bool forward_mouse_button_is_down;


  float pad_move_x;
  float pad_move_y;
  float pad_view_x;
  float pad_view_y;

  bool pad_move_down;
  bool pad_view_down;
  
  float pad_rt;
  float pad_lt;
  bool pad_rb;
  bool pad_lb;

  bool pad_x;
  bool pad_a;
  bool pad_b;
  bool pad_y;
  
  bool pad_d_left;
  bool pad_d_right;
  bool pad_d_up;
  bool pad_d_down;

  bool pad_guide;
  bool pad_share;
  bool pad_menu;
  
  
  std::unordered_map<char, bool> keys_down;
  bool is_key_down(char key) const {
    auto it = keys_down.find(key);
    return it != keys_down.end() && it->second;
  };

  std::unordered_map<std::string, char> keyboard_binds;
  bool is_keyboard_bind_down(const std::string& bind) const {
    auto it = keyboard_binds.find(bind);
    return it != keyboard_binds.end() && is_key_down(it->second);  
  };
};
};