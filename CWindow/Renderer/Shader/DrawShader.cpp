#include "DrawShader.h"

CW::Renderer::DrawShader::DrawShader(const std::string &vertex, const std::string &fragment, const CW::Renderer::Uniform* uniform)
  :vertex(vertex), fragment(fragment), uniform(uniform) {
  compile();
}

CW::Renderer::DrawShader::~DrawShader(){
  destroy();
}

void CW::Renderer::DrawShader::bind() {
  if(!is_compiled) 
    compile();
  glUseProgram(compiledShader);
  if(uniform) 
    uniform->bind(compiledShader);
};

void CW::Renderer::DrawShader::unbind(){
  glUseProgram(0);
}

void CW::Renderer::DrawShader::compile() {
  if(compiledShader) destroy();
  GLuint vertexShaderPart = glCreateShader(GL_VERTEX_SHADER);
  const char* vertexShaderData = vertex.c_str();
  glShaderSource(vertexShaderPart, 1, &vertexShaderData, nullptr);
  glCompileShader(vertexShaderPart);
  GLuint fragmentShaderPart = glCreateShader(GL_FRAGMENT_SHADER);
  const char* fragmentShaderData = fragment.c_str();
  glShaderSource(fragmentShaderPart, 1, &fragmentShaderData, nullptr);
  glCompileShader(fragmentShaderPart);
  compiledShader = glCreateProgram();
  glAttachShader(compiledShader, vertexShaderPart);
  glAttachShader(compiledShader, fragmentShaderPart);
  glLinkProgram(compiledShader);
  glDeleteShader(vertexShaderPart);
  glDeleteShader(fragmentShaderPart);
  is_compiled = true;
}

void CW::Renderer::DrawShader::destroy(){
  glDeleteProgram(compiledShader);
  is_compiled = false;
}
