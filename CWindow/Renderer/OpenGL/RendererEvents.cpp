// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "Renderer.h"
#include "stb_image.h"
static double scroll_x, scroll_y;



void CW::Renderer::Renderer::setWindowMode(CW::Renderer::WindowMode new_mode){
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  
  switch (new_mode) {
    case WindowMode::FULLSCREEN:
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
      windowData.window_mode = WindowMode::FULLSCREEN;
      break;

    case WindowMode::BORDERLESS:
    glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
      windowData.window_mode = WindowMode::BORDERLESS;
      break;
    
    default:
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
      int monitor_w, monitor_h;
      glfwGetMonitorPhysicalSize(monitor, &monitor_w, &monitor_h);
      glfwSetWindowMonitor(window, nullptr, (monitor_w + 800) / 2, (monitor_h + 600) / 2, 800, 600, GLFW_DONT_CARE);
      windowData.window_mode = WindowMode::WINDOW;
      break;
  }
  windowData.window_mode = new_mode;
};



void CW::Renderer::Renderer::setVsync(bool vsync){
  glfwSwapInterval(vsync);
  windowData.vsync = vsync;
};



void CW::Renderer::Renderer::setWindowTitle(const std::string& title){
  glfwSetWindowTitle(window, title.c_str());
  windowData.title = title;
};



// [FEATURE] To change when texture added
void CW::Renderer::Renderer::setIcon(const std::string& path){
  int width, height, channels;
  unsigned char* image = stbi_load(path.c_str(), &width, &height, &channels, 0);

  if(!image) return;
  
  GLFWimage icon;
  icon.width = width;
  icon.height = height;
  icon.pixels = image;

  glfwSetWindowIcon(window, 1, &icon);
  stbi_image_free(image);

  windowData.icon = path;
};



void CW::Renderer::Renderer::minimize(bool minimize) {
  if(minimize){
    glfwIconifyWindow(window);
    windowData.is_minimize = 1;
  }
  else{
    glfwRestoreWindow(window);
    windowData.is_minimize = 0;
  }
};



void CW::Renderer::Renderer::maximize(bool maximize) {
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  if (maximize) {
    glfwSetWindowSize(window, mode->width, mode->height);
    glfwMaximizeWindow(window);
    windowData.is_maximize = 1;
  } 
  else {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    int monitor_w, monitor_h;
    glfwGetMonitorPhysicalSize(monitor, &monitor_w, &monitor_h);
    glfwRestoreWindow(window);
    glfwSetWindowSize(window, 800, 600);
    glfwSetWindowPos(window, (monitor_w + 800) / 2, (monitor_h + 600) / 2);
    windowData.is_maximize = 0;
  }
};



void CW::Renderer::Renderer::setPosition(int x, int y) {
  glfwSetWindowPos(window, x, y);
};



void CW::Renderer::Renderer::setSize(int width, int height) {
  glfwSetWindowSize(window, width, height);
};



void CW::Renderer::Renderer::setCursorVisibility(bool visible) {
  if(visible){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    windowData.is_cursor_visible = 1;
    windowData.is_cursor_on = 1;
  }
  else{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    windowData.is_cursor_visible = 0;
    windowData.is_cursor_on = 1;
  }
};



void CW::Renderer::Renderer::setCursorOn(bool on) {
  if(on){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    windowData.is_cursor_on = 1;
    windowData.is_cursor_visible = 1;
  }
  else{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      windowData.is_cursor_on = 0;
    windowData.is_cursor_visible = 0;
  }
};



void CW::Renderer::Renderer::close(){
  windowData.should_close = 1;
};







void CW::Renderer::Renderer::windowEvents() {
  glfwPollEvents();

  // Update Window Info
  int width, height, x, y;
  GLFWmonitor* monitor = glfwGetWindowMonitor(window);
  bool decorated = glfwGetWindowAttrib(window, GLFW_DECORATED);
  bool maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

  glfwGetFramebufferSize(window, &width, &height);
  glfwGetWindowPos(window, &x, &y);
  glViewport(0, 0, windowData.width, windowData.height);
  
  if(!windowData.should_close) windowData.should_close = glfwWindowShouldClose(window);
  
  if (decorated)
    windowData.window_mode = WindowMode::WINDOW;
  if (maximized && !decorated)
    windowData.window_mode = WindowMode::BORDERLESS; 
  if(!decorated && maximized){
    if(monitor)
      windowData.window_mode = WindowMode::FULLSCREEN;
    else
      windowData.window_mode = WindowMode::WINDOW;
  };

  windowData.is_focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
  windowData.is_minimize = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
  windowData.is_maximize = maximized;

  if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL){
    windowData.is_cursor_visible = 1;
    windowData.is_cursor_on = 1;
  }
  else if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_HIDDEN){
    windowData.is_cursor_visible = 0;
    windowData.is_cursor_on = 1;
  }
  else if(glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED){
    windowData.is_cursor_on = 0;
    windowData.is_cursor_visible = 0;
  }
  
  windowData.width = width;
  windowData.height = height;
  windowData.x = x;
  windowData.y = y;

  // get delta_time
  std::chrono::time_point<std::chrono::high_resolution_clock> new_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> delta = new_time - last_time;
  windowData.delta_time = delta.count();
  last_time = new_time;

  // get mouse input
  double mouse_x, mouse_y;
  glfwGetCursorPos(window, &mouse_x, &mouse_y);
  inputData.mouse_x = mouse_x;
  inputData.mouse_y = mouse_y;
  
  inputData.mouse_scroll_x = scroll_x;
  inputData.mouse_scroll_y = scroll_y;
  scroll_x = 0;
  scroll_y = 0;

  inputData.scroll_is_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);
  inputData.left_mouse_button_is_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
  inputData.right_mouse_button_is_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
  inputData.back_mouse_button_is_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_4);
  inputData.forward_mouse_button_is_down = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_5);

  // get pad input
  if(glfwJoystickPresent(GLFW_JOYSTICK_1)){
    int axesCount;
    int buttonCount;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

    inputData.is_pad_present = 1;

    inputData.pad_move_x = axes[0]; 
    inputData.pad_move_y = -axes[1];
    inputData.pad_view_x = axes[3];
    inputData.pad_view_y = -axes[4]; 
    inputData.pad_lt = (axes[2] + 1) / 2; 
    inputData.pad_rt = (axes[5] + 1) / 2;
    
    inputData.pad_a = buttons[0];
    inputData.pad_b = buttons[1];
    inputData.pad_x = buttons[2];
    inputData.pad_y = buttons[3];

    inputData.pad_lb = buttons[4];
    inputData.pad_rb = buttons[5];

    inputData.pad_guide = buttons[8];
    inputData.pad_share = buttons[6];
    inputData.pad_menu = buttons[7];
    inputData.pad_move_down = buttons[9];
    inputData.pad_view_down = buttons[10];

    inputData.pad_d_up = buttons[15];
    inputData.pad_d_right = buttons[16];
    inputData.pad_d_down = buttons[17];
    inputData.pad_d_left = buttons[18];
  }
  else{
    inputData.is_pad_present = 0;

    inputData.pad_move_x = 0; 
    inputData.pad_move_y = 0;
    inputData.pad_view_x = 0;
    inputData.pad_view_y = 0; 
    inputData.pad_lt = 0; 
    inputData.pad_rt = 0;
    
    inputData.pad_a = 0;
    inputData.pad_b = 0;
    inputData.pad_x = 0;
    inputData.pad_y = 0;

    inputData.pad_lb = 0;
    inputData.pad_rb = 0;

    inputData.pad_guide = 0;
    inputData.pad_share = 0;
    inputData.pad_menu = 0;
    inputData.pad_move_down = 0;
    inputData.pad_view_down = 0;

    inputData.pad_d_up = 0;
    inputData.pad_d_right = 0;
    inputData.pad_d_down = 0;
    inputData.pad_d_left = 0;
  }

  // get keyboard
  inputData.keys_down.clear();

  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) inputData.keys_down["A"] = true;
  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) inputData.keys_down["B"] = true;
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) inputData.keys_down["C"] = true;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) inputData.keys_down["D"] = true;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) inputData.keys_down["E"] = true;
  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) inputData.keys_down["F"] = true;
  if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) inputData.keys_down["G"] = true;
  if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) inputData.keys_down["H"] = true;
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) inputData.keys_down["I"] = true;
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) inputData.keys_down["J"] = true;
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) inputData.keys_down["K"] = true;
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) inputData.keys_down["L"] = true;
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) inputData.keys_down["M"] = true;
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) inputData.keys_down["N"] = true;
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) inputData.keys_down["O"] = true;
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) inputData.keys_down["P"] = true;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) inputData.keys_down["Q"] = true;
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) inputData.keys_down["R"] = true;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) inputData.keys_down["S"] = true;
  if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) inputData.keys_down["T"] = true;
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) inputData.keys_down["U"] = true;
  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) inputData.keys_down["V"] = true;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) inputData.keys_down["W"] = true;
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) inputData.keys_down["X"] = true;
  if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) inputData.keys_down["Y"] = true;
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) inputData.keys_down["Z"] = true;

  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) inputData.keys_down["0"] = true;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) inputData.keys_down["1"] = true;
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) inputData.keys_down["2"] = true;
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) inputData.keys_down["3"] = true;
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) inputData.keys_down["4"] = true;
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) inputData.keys_down["5"] = true;
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) inputData.keys_down["6"] = true;
  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) inputData.keys_down["7"] = true;
  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) inputData.keys_down["8"] = true;
  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) inputData.keys_down["9"] = true;

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)        inputData.keys_down[" "] = true;
  if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)        inputData.keys_down[","] = true;
  if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)       inputData.keys_down["."] = true;
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)        inputData.keys_down["-"] = true;
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)        inputData.keys_down["="] = true;
  if (glfwGetKey(window, GLFW_KEY_SLASH) == GLFW_PRESS)        inputData.keys_down["/"] = true;
  if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS)    inputData.keys_down[";"] = true;
  if (glfwGetKey(window, GLFW_KEY_APOSTROPHE) == GLFW_PRESS)   inputData.keys_down["'"] = true;
  if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS) inputData.keys_down["["] = true;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)inputData.keys_down["]"] = true;
  if (glfwGetKey(window, GLFW_KEY_BACKSLASH) == GLFW_PRESS)    inputData.keys_down["\\"] = true;
  if (glfwGetKey(window, GLFW_KEY_GRAVE_ACCENT) == GLFW_PRESS) inputData.keys_down["`"] = true;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)     inputData.keys_down["ESC"] = true;
  if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)      inputData.keys_down["ENTER"] = true;
  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)        inputData.keys_down["TAB"] = true;
  if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)  inputData.keys_down["BACKSPACE"] = true;
  if (glfwGetKey(window, GLFW_KEY_DELETE) == GLFW_PRESS)     inputData.keys_down["DELETE"] = true;
  if (glfwGetKey(window, GLFW_KEY_INSERT) == GLFW_PRESS)     inputData.keys_down["INSERT"] = true;

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)    inputData.keys_down["UP"] = true;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)  inputData.keys_down["DOWN"] = true;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)  inputData.keys_down["LEFT"] = true;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) inputData.keys_down["RIGHT"] = true;

  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)   inputData.keys_down["LSHIFT"] = true;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)  inputData.keys_down["RSHIFT"] = true;
  if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) inputData.keys_down["LCTRL"] = true;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)inputData.keys_down["RCTRL"] = true;
  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)     inputData.keys_down["LALT"] = true;
  if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)    inputData.keys_down["RALT"] = true;

  if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)  inputData.keys_down["F1"] = true;
  if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)  inputData.keys_down["F2"] = true;
  if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)  inputData.keys_down["F3"] = true;
  if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)  inputData.keys_down["F4"] = true;
  if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)  inputData.keys_down["F5"] = true;
  if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)  inputData.keys_down["F6"] = true;
  if (glfwGetKey(window, GLFW_KEY_F7) == GLFW_PRESS)  inputData.keys_down["F7"] = true;
  if (glfwGetKey(window, GLFW_KEY_F8) == GLFW_PRESS)  inputData.keys_down["F8"] = true;
  if (glfwGetKey(window, GLFW_KEY_F9) == GLFW_PRESS)  inputData.keys_down["F9"] = true;
  if (glfwGetKey(window, GLFW_KEY_F10) == GLFW_PRESS) inputData.keys_down["F10"] = true;
  if (glfwGetKey(window, GLFW_KEY_F11) == GLFW_PRESS) inputData.keys_down["F11"] = true;
  if (glfwGetKey(window, GLFW_KEY_F12) == GLFW_PRESS) inputData.keys_down["F12"] = true;
};
