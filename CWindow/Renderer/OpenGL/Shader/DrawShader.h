#pragma once
#include "../Uniform/Uniform.h"
#include "DrawShaderData.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <unordered_map>







namespace CW::Renderer{
class DrawShader{
private:
  GLuint compiledShader = 0;
  std::unordered_map<GLenum, CW::Renderer::DrawShaderData> registerShader;
  
  std::vector<const CW::Renderer::Uniform*> uniforms;
  bool is_compiled = false;

public:
  DrawShader();
  DrawShader(const std::string& vertex, const std::string& fragment);
  ~DrawShader();

  void setVertexShader(const std::string& shader);
  void setFragmentShader(const std::string& shader);

  void addShader(const std::string& shader, GLuint type);

  void compile();
  void destroy();

  void bind();
  void unbind();
  
  std::vector<const CW::Renderer::Uniform*>& getUniforms();

};
};

#include "ComputeShader.hpp"