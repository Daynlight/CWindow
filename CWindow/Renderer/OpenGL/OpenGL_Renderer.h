#pragma once
#include "Macro.h"
#include "../iRenderer.h"
#include "../Mesh/Mesh.h"
#include "../Shader/DrawShader.h"
#include "../Shader/ComputeShader.h"
#include "../Uniform/Uniform.h"
#include "../Data/WindowData.h"
#include "../Data/InputData.h"

#include "glad/glad.h"

#include <vector>
#include <stdexcept>
#include <functional>
#include <chrono>

namespace CW::Renderer{
class Renderer : public iRenderer {
private:
  APIWindow* window;
  std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();
  
  WindowData windowData;
  InputData inputData;
  
public:
  Renderer();
  ~Renderer();

  const WindowData* getWindowData();
  const InputData* getInputData();
  
  void createWindow();
  APIWindow* getWindow();
  void createRenderer();
  void beginFrame();
  void swapBuffer();
  
  void windowEvents();
  void setWindowMode(CW::Renderer::WindowMode mode);
  void setWindowTitle(const std::string& title);
  void setVsync(bool vsync);
  void minimizedSwitch();
  void maximizeSwitch();
};
};

#include "OpenGL_Renderer.h"