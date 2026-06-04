#pragma once
#include "../../Uniform/Uniform.h"
#include "ShaderData.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <vector>
#include <unordered_map>







namespace CW::Renderer{
class Shader{
private:
  GLuint compiledShader = 0;
  std::unordered_map<GLenum, CW::Renderer::ShaderData> registerShader;
  
  std::vector<const CW::Renderer::Uniform *> uniforms;
  
  bool is_compiled = false;

public:
  Shader() noexcept;
  Shader(const std::string& vertex, const std::string& fragment) noexcept;
  ~Shader() noexcept;

  void setVertexShader(const std::string& source) noexcept;
  void setFragmentShader(const std::string& source) noexcept;

  void setShader(const std::string& source, GLuint type) noexcept;
  const std::unordered_map<GLenum, CW::Renderer::ShaderData>& getShaders();
  void removeShaders(GLuint type) noexcept;
  void clearShaders() noexcept;

  void compile() noexcept;
  void destroy() noexcept;

  void bind() noexcept;
  void unbind() const noexcept;
  
  std::vector<const CW::Renderer::Uniform*>& getUniforms() noexcept;

};
};
