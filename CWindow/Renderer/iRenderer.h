#pragma once
#include "../Macro.h"
#include "../vendor/glm/glm/glm.hpp"
#include <string>
#include <vector>
#include <functional>

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

class iRenderer{
public:
  virtual ~iRenderer() = default;

  virtual void createWindow() = 0;
  virtual APIWindow* getWindow() = 0;
  virtual WindowData* getWindowData() = 0;

  virtual void windowEvents() = 0;
  
  virtual void createRenderer() = 0;
  virtual void renderFrame() = 0;
  virtual void swapBuffer() = 0;

  virtual void bindVertexShader(std::string shader) = 0;
  virtual void bindFragmentShader(std::string shader) = 0;
  virtual void compileShaders() = 0;

  virtual void bindComputeShader(std::string shader) = 0;
  virtual void runComputeShader(std::vector<float> data) = 0;
};
};