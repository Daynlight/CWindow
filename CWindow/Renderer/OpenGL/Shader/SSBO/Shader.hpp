#include "Shader.h"







template<typename T>
void CW::Renderer::GPUStore::set(const std::vector<T>& data) noexcept {
  if (data.empty())
    return;

  if(!is_created) 
    create();

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO);
  glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  data_size = data.size();
};

