#pragma once
#include "Macro.h"
#include "../iRenderer.h"
#include "../WindowData.h"

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

  GLuint VAO, VBO, EBO;
  GLuint pointsSSBO;
  std::string vertexShader = "";
  std::string fragmentShader = "";
  std::string computeShader = "";
  GLuint compiledShader;

  WindowData windowData;
  WindowData lastWindowData;
  bool init_update = true;

public:
  Renderer();
  ~Renderer();

  void windowEvents();
  WindowData* getWindowData();
  
  void createWindow();
  APIWindow* getWindow();
  void createRenderer();
  void renderFrame();
  void swapBuffer();

  void bindVertexShader(std::string shader);
  void bindFragmentShader(std::string shader);
  void compileShaders();
  void bindComputeShader(std::string shader);
  void runComputeShader(std::vector<float> data);
  
};
};

#include "OpenGL_Renderer.h"