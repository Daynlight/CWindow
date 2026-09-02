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
  is_compiled = false;
};



CW::Renderer::Shader::Shader(const CW::Renderer::Shader& second) noexcept
  : compiledShader(second.compiledShader),
    registerShader(second.registerShader),
    uniforms(second.uniforms),
    is_compiled(second.is_compiled) {};



CW::Renderer::Shader& CW::Renderer::Shader::operator=(const CW::Renderer::Shader& second) noexcept {
  if(this == &second) return *this;
  
  compiledShader = second.compiledShader;
  registerShader = second.registerShader;
  uniforms = second.uniforms;
  is_compiled = second.is_compiled;
  
  return *this;
};



CW::Renderer::Shader::Shader(CW::Renderer::Shader&& second) noexcept
  : compiledShader(std::move(second.compiledShader)),
    registerShader(std::move(second.registerShader)),
    uniforms(std::move(second.uniforms)),
    is_compiled(std::move(second.is_compiled)){
  second.compiledShader = 0;
  second.is_compiled = false;
};



CW::Renderer::Shader& CW::Renderer::Shader::operator=(CW::Renderer::Shader&& second) noexcept {
  if(this == &second) return *this;
  
  compiledShader = std::move(second.compiledShader);
  registerShader = std::move(second.registerShader);
  uniforms = std::move(second.uniforms);
  is_compiled = std::move(second.is_compiled);

  second.compiledShader = 0;
  second.is_compiled = false;

  return *this;
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



const std::unordered_map<GLenum, CW::Renderer::ShaderData>& CW::Renderer::Shader::getRegisterShader() const{
  return registerShader;
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

  for(CW::Renderer::Uniform* uniform : uniforms) 
    uniform->bind(compiledShader);
};



void CW::Renderer::Shader::unbind() const noexcept {
  glUseProgram(0);
};


GLuint CW::Renderer::Shader::getShaderProgram(){
  return compiledShader;
};


std::vector<CW::Renderer::Uniform*>& CW::Renderer::Shader::getUniforms() noexcept {
  return uniforms;
};
