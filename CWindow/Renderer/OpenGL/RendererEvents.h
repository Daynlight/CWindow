#pragma once 

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
}

void CW::Renderer::Renderer::setVsync(bool vsync){
  glfwSwapInterval(vsync);
  windowData.vsync = vsync;
}

void CW::Renderer::Renderer::setWindowTitle(const std::string& title){
  glfwSetWindowTitle(window, title.c_str());
  windowData.title = title;
}


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
}

void CW::Renderer::Renderer::minimize(bool minimize) {
  if(minimize){
    glfwIconifyWindow(window);
    windowData.is_minimize = 1;
  }
  else{
    glfwRestoreWindow(window);
    windowData.is_minimize = 0;
  }
}

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
}

void CW::Renderer::Renderer::setSize(int width, int height) {
  glfwSetWindowSize(window, width, height);
}

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
}

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
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
      inputData.keys_down[static_cast<char>(key)] = true;
    };
  };
};
