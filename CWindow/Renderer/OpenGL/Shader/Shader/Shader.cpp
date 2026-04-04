#include "Shader.h"







CW::Renderer::Shader::Shader() noexcept
  :is_compiled(false) {};



CW::Renderer::Shader::Shader(const std::string& vertex_source, const std::string& fragment_source) noexcept
  :is_compiled(false) {
  setVertexShader(vertex_source);
  setFragmentShader(fragment_source);
};



CW::Renderer::Shader::~Shader() noexcept {
  destroy();
};



void CW::Renderer::Shader::setVertexShader(const std::string& source) noexcept {
  setShader(source, GL_VERTEX_SHADER);
  is_compiled = false;
};



void CW::Renderer::Shader::setFragmentShader(const std::string& source) noexcept {
  setShader(source, GL_FRAGMENT_SHADER);
  is_compiled = false;
};



void CW::Renderer::Shader::setShader(const std::string& source, const GLuint type) noexcept {
  this->registerShader[type] = CW::Renderer::ShaderData(source, type);
  is_compiled = false;
};



void CW::Renderer::Shader::removeShaders(GLuint type) noexcept {
  this->registerShader.erase(type);
  is_compiled = false;
};



void CW::Renderer::Shader::clearShaders() noexcept {
  this->registerShader.clear();
  is_compiled = false;
};




void CW::Renderer::Shader::compile() noexcept {
  if(is_compiled) 
  destroy();
  
  compiledShader = glCreateProgram();

  for(std::pair<const GLenum, CW::Renderer::ShaderData>& shader : registerShader){
    GLuint compiled_shader = shader.second.getCompiledShader();
    glAttachShader(compiledShader, compiled_shader);
  };
  
  glLinkProgram(compiledShader);

  for(std::pair<const GLenum, CW::Renderer::ShaderData>& shader : registerShader)
    shader.second.deleteCompiledShader();

  is_compiled = true;
};



void CW::Renderer::Shader::destroy() noexcept {
  if(is_compiled){
    glDeleteProgram(compiledShader);
    compiledShader = 0;
  };

  is_compiled = false;
};



void CW::Renderer::Shader::bind() noexcept {
  if(!is_compiled) compile();
  
  glUseProgram(compiledShader);

  for(const CW::Renderer::Uniform* uniform : uniforms) 
    uniform->bind(compiledShader);
};



void CW::Renderer::Shader::unbind() const noexcept {
  glUseProgram(0);
};



std::vector<const CW::Renderer::Uniform*>& CW::Renderer::Shader::getUniforms() noexcept {
  return uniforms;
};
