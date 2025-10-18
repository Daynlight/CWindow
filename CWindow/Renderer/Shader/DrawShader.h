#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>

namespace CW::Renderer{
class DrawShader{
private:
  GLuint compiledShader;
  bool is_compiled = false;
  std::string vertex = "";
  std::string fragment = "";

public:
  DrawShader(const std::string& vertex, const std::string& fragment);
  ~DrawShader();
  void render();
  void compile();
  void destroy();
};
};