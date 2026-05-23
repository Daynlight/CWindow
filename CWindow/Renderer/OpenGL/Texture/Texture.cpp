#include "Texture.h"







CW::Renderer::Texture::Texture() : is_compiled(false) {};



CW::Renderer::Texture::Texture(TextureData data, GLint min_filter, GLint max_filter)
: is_compiled(false){
  compile(data, min_filter, max_filter);
};



CW::Renderer::Texture::~Texture() {
  destroy();
};



void CW::Renderer::Texture::compile(TextureData data, GLint min_filter, GLint max_filter) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);

  glTexImage2D(GL_TEXTURE_2D, 0, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0); 
  
  is_compiled = true;
};



void CW::Renderer::Texture::destroy(){
  if(is_compiled)
    glDeleteTextures(1, &texture);
  
  is_compiled = false;
}



void CW::Renderer::Texture::bind(unsigned int socket) {
  if(!is_compiled) 
    return;

  glActiveTexture(GL_TEXTURE0 + socket);
  glBindTexture(GL_TEXTURE_2D, texture);
};



void CW::Renderer::Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
};
