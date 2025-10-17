#pragma once
#include "../Macro.h"
#include "WindowData.h"
#include "../vendor/glm/glm/glm.hpp"
#include <string>
#include <vector>
#include <functional>

namespace CW::Renderer{
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