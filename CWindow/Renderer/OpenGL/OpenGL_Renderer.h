#pragma once
#include "Macro.h"
#include "../iRenderer.h"

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
  std::pair<float, float> global_position = {0.0f, 0.0f};
  float zoom  = 1.0f;
  APIWindow* window;
  bool running = true;

  GLuint VAO, VBO, EBO;
  GLuint pointsSSBO;
  std::string vertexShader = "";
  std::string fragmentShader = "";
  std::string computeShader = "";
  GLuint compiledShader;

public:
  Renderer();
  ~Renderer();

  void windowMovement(std::pair<float, float> move);
  void windowZoom(float zoom);

  void windowEvents();
  
  void createWindow();
  APIWindow* getWindow();
  void createRenderer();
  bool isRunning();
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