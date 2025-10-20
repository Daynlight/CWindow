#pragma once 
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>

namespace CW::Renderer{
class ComputeShader{
private:
  GLuint SSBO;
  GLuint compiledShader;
  std::string compute_shader = "";
  bool is_compiled = false;
  bool save_on_gup = false;

public:
  ComputeShader(const std::string& compute_shader, bool save_on_gup = false);
  ~ComputeShader();

  void compile();
  void destroy();

  void run(std::vector<float> data, std::vector<float>* return_data = nullptr); 
};
};