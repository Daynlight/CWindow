#pragma once
#include "../Uniform/Uniform.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>

namespace CW::Renderer{
class DrawShader{
private:
  GLuint compiledShader;
  std::string vertex = "";
  std::string fragment = "";
  bool is_compiled = false;
  std::vector<const CW::Renderer::Uniform*> uniforms;

public:
  DrawShader(const std::string& vertex, const std::string& fragment);
  ~DrawShader();

  void bind();
  void unbind();
  
  void compile();
  void destroy();
  
  void setVertexShader(const std::string& shader);
  void setFragmentShader(const std::string& shader);
  
  std::vector<const CW::Renderer::Uniform*>& getUniforms();
};
};