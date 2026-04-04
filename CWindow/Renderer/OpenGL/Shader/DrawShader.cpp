#include "DrawShader.h"

CW::Renderer::DrawShader::DrawShader()
  :is_compiled(false) {}


CW::Renderer::DrawShader::DrawShader(const std::string &vertex, const std::string &fragment) 
  :is_compiled(false) {
  setVertexShader(vertex);
  setFragmentShader(fragment);
};

CW::Renderer::DrawShader::~DrawShader(){
  destroy();
}

void CW::Renderer::DrawShader::bind() {
  if(!is_compiled) 
    compile();
  
  glUseProgram(compiledShader);

  for(const CW::Renderer::Uniform* uniform : uniforms)
    uniform->bind(compiledShader);
};

void CW::Renderer::DrawShader::unbind(){
  glUseProgram(0);
};

void CW::Renderer::DrawShader::compile() {
  if(is_compiled) 
  destroy();
  
  compiledShader = glCreateProgram();

  for(std::pair<const GLenum, CW::Renderer::DrawShaderData>& shader : registerShader){
    GLuint compiled_shader = shader.second.getCompiledShader();
    glAttachShader(compiledShader, compiled_shader);
  };
  
  glLinkProgram(compiledShader);
  
  for(std::pair<const GLenum, CW::Renderer::DrawShaderData>& shader : registerShader){
    shader.second.deleteShader();
  };

  is_compiled = true;
};

void CW::Renderer::DrawShader::destroy(){
  if(compiledShader){
    glDeleteProgram(compiledShader);
    compiledShader = 0;
  };
  is_compiled = false;
}

std::vector<const CW::Renderer::Uniform *> &CW::Renderer::DrawShader::getUniforms(){
  return uniforms;
}

void CW::Renderer::DrawShader::setVertexShader(const std::string &shader){
  addShader(shader, GL_VERTEX_SHADER);
  is_compiled = false;
}

void CW::Renderer::DrawShader::setFragmentShader(const std::string &shader){
  addShader(shader, GL_FRAGMENT_SHADER);
  is_compiled = false;
};

void CW::Renderer::DrawShader::addShader(const std::string &shader, GLuint type){
  this->registerShader[type] = CW::Renderer::DrawShaderData(shader, type);
  is_compiled = false;
};
