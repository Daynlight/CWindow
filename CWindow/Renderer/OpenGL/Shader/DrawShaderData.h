#pragma once

#include <glad/glad.h>

#include <string>







namespace CW::Renderer{
class DrawShaderData{
private:
  std::string source = "";
  GLenum type;
  GLuint shaderPart = 0;

public:
  DrawShaderData();
  DrawShaderData(std::string source, GLenum type);

  GLuint getCompiledShader();

  void deleteShader();

};
};