#include "ComputeShader.h"

CW::Renderer::ComputeShader::ComputeShader(const std::string &compute_shader, bool save_on_gup)
  :compute_shader(compute_shader), save_on_gup(save_on_gup){
  compile();
}

CW::Renderer::ComputeShader::~ComputeShader() {
  destroy();
}

void CW::Renderer::ComputeShader::run(std::vector<float> data, std::vector<float>* return_data) {
  glUseProgram(compiledShader);
    
  glGenBuffers(1, &pointsSSBO);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pointsSSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(glm::vec2), data.data(), GL_STATIC_DRAW);

  glDispatchCompute(data.size(), 1, 1);

  if(save_on_gup) 
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
};

void CW::Renderer::ComputeShader::compile() {
  GLuint computeShaderPart = glCreateShader(GL_COMPUTE_SHADER);
  const char* computeShaderData = compute_shader.c_str();
  glShaderSource(computeShaderPart, 1, &computeShaderData, nullptr);
  glCompileShader(computeShaderPart);

  compiledShader = glCreateProgram();
  glAttachShader(compiledShader, computeShaderPart);
  glLinkProgram(compiledShader);
  
  glDeleteShader(computeShaderPart);
  is_compiled = true;
}

void CW::Renderer::ComputeShader::destroy() {
  glDeleteProgram(compiledShader);
  
  if (pointsSSBO) 
    glDeleteBuffers(1, &pointsSSBO);

  is_compiled = false;
};
