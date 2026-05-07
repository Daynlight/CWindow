#include "ShaderData.h"







CW::Renderer::ShaderData::ShaderData() noexcept
  :is_compiled(false) {};



CW::Renderer::ShaderData::ShaderData(std::string source, const GLenum type) noexcept
  :source(std::move(source)), type(type), is_compiled(false) {};



GLuint CW::Renderer::ShaderData::getCompiledShader() noexcept {
  if(is_compiled) return shaderPart;

  GLuint shaderPart = glCreateShader(type);
  const char* shaderData = source.c_str();
  glShaderSource(shaderPart, 1, &shaderData, nullptr);
  glCompileShader(shaderPart);

  is_compiled = true;
  return shaderPart;
};



void CW::Renderer::ShaderData::deleteCompiledShader() noexcept {
  glDeleteShader(shaderPart);
  shaderPart = 0;
  is_compiled = false;
};
