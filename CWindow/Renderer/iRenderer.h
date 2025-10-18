#pragma once
#include "../Macro.h"
#include "Data/WindowData.h"
#include "Data/InputData.h"
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
  virtual const WindowData* getWindowData() = 0;
  virtual const InputData* getInputData() = 0;

  virtual void createRenderer() = 0;
  virtual void beginFrame() = 0;
  virtual void swapBuffer() = 0;
  
  virtual void windowEvents() = 0;
  virtual void setWindowMode(CW::Renderer::WindowMode mode) = 0;
  virtual void setWindowTitle(const std::string& title) = 0;
  virtual void setVsync(bool vsync) = 0;
  virtual void minimizedSwitch() = 0;
  virtual void maximizeSwitch() = 0;
};
};