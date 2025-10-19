#pragma once
#include "../Uniform/Uniform.h"

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
  const CW::Renderer::Uniform* uniform = nullptr;

public:
  DrawShader(const std::string& vertex, const std::string& fragment, const CW::Renderer::Uniform* uniform = nullptr);
  ~DrawShader();
  void bind();
  void unbind();
  void compile();
  void destroy();
};
};