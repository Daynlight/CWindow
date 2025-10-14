#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include <stdexcept>

namespace OpenGLRenderer{
class Renderer{
private:
  std::pair<float, float> global_position = {0.0f, 0.0f};
  float zoom  = 1.0f;
  GLFWwindow* window;
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
  GLFWwindow* getWindow();
  void createRenderer();
  bool isRunning();
  void renderFrame();
  void swapBuffer();

  void bindVertexShader(std::string shader);
  void bindFragmentShader(std::string shader);
  void compileShaders();
  void bindComputeShader(std::string shader);
  void runComputeShader(std::vector<glm::vec2> data);
  
};
};