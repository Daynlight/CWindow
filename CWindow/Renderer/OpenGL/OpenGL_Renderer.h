#pragma once
#include "Macro.h"
#include "../iRenderer.h"
#include "../Mesh/Mesh.h"
#include "../Shader/DrawShader.h"
#include "../Data/WindowData.h"
#include "../Data/InputData.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>

namespace CW::Renderer{
class Renderer : public iRenderer {
private:
  APIWindow* window;

  GLuint pointsSSBO;
  std::string computeShader = "";
  GLuint compiledShader;
  
  WindowData windowData;
  InputData inputData;
  
public:
  Renderer();
  ~Renderer();

  void windowEvents();
  const WindowData* getWindowData();
  const InputData* getInputData();
  
  void createWindow();
  APIWindow* getWindow();
  void createRenderer();
  void beginFrame();
  void swapBuffer();

  void setWindowMode(CW::Renderer::WindowMode mode);
  void setVsync(bool vsync);
  void setWindowTitle(const std::string& title);
  void minimizedSwitch();
  void maximizeSwitch();

  void bindComputeShader(std::string shader);
  void runComputeShader(std::vector<float> data);
  
};
};

#include "OpenGL_Renderer.h"