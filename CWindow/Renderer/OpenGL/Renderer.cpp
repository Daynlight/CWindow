#include "Renderer.h"

static double scroll_x, scroll_y;

void CW::Renderer::Renderer::setWindowMode(CW::Renderer::WindowMode new_mode){
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  
  switch (new_mode) {
    case WindowMode::FULLSCREEN:
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
      glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
      break;

    case WindowMode::BOARDLESS:
      glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
      break;
    
    default:
      glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
      glfwSetWindowMonitor(window, nullptr, (windowData.x + windowData.width) / 2, (windowData.y + windowData.height) / 2, 
      windowData.width / 2, windowData.height / 2, GLFW_DONT_CARE);
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

void CW::Renderer::Renderer::minimizedSwitch() {
  if(windowData.is_minimize){
    glfwIconifyWindow(window);
  }
  else{
    glfwRestoreWindow(window);
  }
}

void CW::Renderer::Renderer::maximizeSwitch() {
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);

  if (windowData.is_maximize) {
    glfwSetWindowSize(window, mode->width, mode->height);
    glfwMaximizeWindow(window);
  } 
  else {
    glfwRestoreWindow(window);
  }
};





CW::Renderer::Renderer::Renderer(bool windowless) { 
  if(!windowless) createWindow(); 
  else windowLessRenderer();
  createRenderer();
};

CW::Renderer::Renderer::~Renderer() {
  windowData.should_close = false;
  if (window) glfwDestroyWindow(window);
  glfwTerminate();
};

void CW::Renderer::Renderer::windowEvents() {
  glfwPollEvents();

  // Update Window Info
  int width, height, x, y;
  glfwGetFramebufferSize(window, &width, &height);
  glfwGetWindowPos(window, &x, &y);
  glViewport(0, 0, windowData.width, windowData.height);
  windowData.should_close = !glfwWindowShouldClose(window);
  windowData.is_focused = glfwGetWindowAttrib(window, GLFW_FOCUSED);
  windowData.is_minimize = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
  windowData.is_maximize = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
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

  // get keyboard
  inputData.keys_down.clear();
  for (int key = GLFW_KEY_SPACE; key <= GLFW_KEY_LAST; ++key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
      inputData.keys_down[static_cast<char>(key)] = true;
    };
  };
};

const CW::Renderer::WindowData *CW::Renderer::Renderer::getWindowData() {
  return &windowData;
}

const CW::Renderer::InputData *CW::Renderer::Renderer::getInputData() {
  return &inputData;
}

void CW::Renderer::Renderer::setKeyboardBind(const std::string &action, char key){
  inputData.keyboard_binds[action] = key;
}

void CW::Renderer::Renderer::createWindow()
{

  if(!glfwInit()) {
    windowData.should_close = false;
    glfwTerminate();
    throw std::runtime_error("Can't initialize GLFW");
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  window = glfwCreateWindow(800, 600, windowData.title.c_str(), nullptr, nullptr);
  if(!window) {
    windowData.should_close = false;
    glfwTerminate();
    throw std::runtime_error("Can't Create Window");
  };

  glfwMakeContextCurrent(window);

  glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
    scroll_x = xOffset;
    scroll_y = yOffset;
  });
}

void CW::Renderer::Renderer::windowLessRenderer() {
  if (!glfwInit()) {
    windowData.should_close = false;
    throw std::runtime_error("Can't initialize GLFW");
  }

  // Tell GLFW to make the window invisible
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

  // Create a tiny, hidden window just for the context
  window = glfwCreateWindow(1, 1, "", nullptr, nullptr);
  if (!window) {
    windowData.should_close = false;
    glfwTerminate();
    throw std::runtime_error("Can't create hidden window for headless renderer");
  }

  glfwMakeContextCurrent(window); // Make context current
}


APIWindow* CW::Renderer::Renderer::getWindow() {
  return window;
}

void CW::Renderer::Renderer::createRenderer()
{
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    windowData.should_close = false;
    return;
  };

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  windowEvents();
}

void CW::Renderer::Renderer::beginFrame() {
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
}

void CW::Renderer::Renderer::swapBuffer() {
  glfwSwapBuffers(window);
}