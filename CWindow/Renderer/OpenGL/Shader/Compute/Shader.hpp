#include "Shader.h"







template<typename T>
void CW::Renderer::ComputeShader::run(std::vector<T> data, unsigned int x, unsigned int y, unsigned int z) noexcept {
  if(!is_compiled) compile();
    
  glUseProgram(compiledShader);

  glGenBuffers(1, &SSBO);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);

  glDispatchCompute(x, y, z);
  
  data_size = data.size();
};



template<typename T>
std::vector<T> CW::Renderer::ComputeShader::get() const noexcept {
  std::vector<T> data(data_size);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data_size * sizeof(T), data.data());

  return data;
};
