#include "Shader.h"







CW::Renderer::GPUStore::GPUStore() noexcept
  :is_created(false) {};



CW::Renderer::GPUStore::~GPUStore() noexcept {
  destroy();
};


void CW::Renderer::GPUStore::create() noexcept{
  if(is_created) 
    destroy();

  glGenBuffers(1, &SSBO);
  is_created = true;
}

void CW::Renderer::GPUStore::destroy() noexcept {
  if (is_created){
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, 0);
    glDeleteBuffers(1, &SSBO);
    SSBO = 0;
    slot = 0;
  };
      
  is_created = false;
};



void CW::Renderer::GPUStore::bind(GLuint socket) noexcept{
  this->slot = socket;
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, socket, SSBO);
};



void CW::Renderer::GPUStore::unbind() noexcept{
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, 0);
  slot = 0;
};
