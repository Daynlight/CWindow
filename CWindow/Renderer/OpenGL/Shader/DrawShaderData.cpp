#include "DrawShaderData.h"

CW::Renderer::DrawShaderData::DrawShaderData(){};

CW::Renderer::DrawShaderData::DrawShaderData(std::string source, GLenum type) 
  :source(std::move(source)), type(type) {};

GLuint CW::Renderer::DrawShaderData::getCompiledShader() {
  if(shaderPart) return shaderPart;

  shaderPart = glCreateShader(type);
  const char* shaderData = source.c_str();
  glShaderSource(shaderPart, 1, &shaderData, nullptr);
  glCompileShader(shaderPart);

  return shaderPart;
};


void CW::Renderer::DrawShaderData::deleteShader() {
  glDeleteShader(shaderPart);
  shaderPart = 0;
};
