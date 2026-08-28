#pragma once
#include "../../Macro.h"

#include "../Shared/Data/WindowData.h"
#include "../Shared/Data/InputData.h"

#include "glm/glm.hpp"

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
  virtual void setIcon(const std::string& path) = 0;
  virtual void setVsync(bool vsync) = 0;
  virtual void minimize(bool minimize) = 0;
  virtual void maximize(bool maximize) = 0;
  virtual void setPosition(int x, int y) = 0;
  virtual void setSize(int width, int height) = 0;
  virtual void setCursorVisibility(bool visible) = 0;
  virtual void setCursorOn(bool on) = 0;
  virtual void close() = 0;
};
};