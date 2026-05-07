#include "Shader.h"







CW::Renderer::ComputeShader::ComputeShader(const std::string &compute_shader) noexcept
  :compute_shader(compute_shader), is_compiled(false) {};



CW::Renderer::ComputeShader::~ComputeShader() noexcept {
  destroy();
};



void CW::Renderer::ComputeShader::compile() noexcept {
  if(is_compiled) destroy();
    
  GLuint computeShaderPart = glCreateShader(GL_COMPUTE_SHADER);
  const char* computeShaderData = compute_shader.c_str();
  glShaderSource(computeShaderPart, 1, &computeShaderData, nullptr);
  glCompileShader(computeShaderPart);

  compiledShader = glCreateProgram();
  glAttachShader(compiledShader, computeShaderPart);
  glLinkProgram(compiledShader);
  
  glDeleteShader(computeShaderPart);

  is_compiled = true;
};



void CW::Renderer::ComputeShader::destroy() noexcept {
  if (SSBO){
    glDeleteBuffers(1, &SSBO);
    SSBO = 0;
  };
    
  if(is_compiled){
    glDeleteProgram(compiledShader);
    compiledShader = 0;
  };
      
  is_compiled = false;
};
