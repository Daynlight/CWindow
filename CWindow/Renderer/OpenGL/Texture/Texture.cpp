#include "Texture.h"

CW::Renderer::Texture::Texture(){
};

CW::Renderer::Texture::~Texture() {
  glDeleteTextures(1, &texture);
};

void CW::Renderer::Texture::create(TextureLoader data) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0); 
};

void CW::Renderer::Texture::bind(unsigned int socket) {
  glActiveTexture(GL_TEXTURE0 + socket);
  glBindTexture(GL_TEXTURE_2D, texture);
};

void CW::Renderer::Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
};