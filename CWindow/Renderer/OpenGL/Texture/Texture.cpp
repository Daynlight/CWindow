#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

CW::Renderer::Texture::Texture(){
};

CW::Renderer::Texture::~Texture() {
  glDeleteTextures(0, &texture);
};

bool CW::Renderer::Texture::load(const std::string &path) {
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if(!data) return 1;
  
  GLenum format = GL_RGB;
  if (channels == 1) format = GL_RED;
  else if (channels == 3) format = GL_RGB;
  else if (channels == 4) format = GL_RGBA;

  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0); 

  stbi_image_free(data);

  return 0;
};

void CW::Renderer::Texture::bind() {
  glBindTexture(GL_TEXTURE_2D, texture);
};

void CW::Renderer::Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
};