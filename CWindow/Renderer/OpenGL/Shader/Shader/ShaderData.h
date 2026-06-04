#pragma once

#include <glad/glad.h>

#include <string>







namespace CW::Renderer{
class ShaderData{
private:
  std::string source = "";
  GLenum type = 0;

  GLuint shaderPart = 0;
  bool is_compiled = false;

public:
  ShaderData() noexcept;
  ShaderData(std::string source, const GLenum type) noexcept;

  GLuint getCompiledShader() noexcept;
  std::string getSource() const;
  void deleteCompiledShader() noexcept;

};
};