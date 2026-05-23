#pragma once
#include "glad/glad.h"

#include <string>

#include "TextureLoader.h"
#include "TextureData.h"







namespace CW::Renderer{
class Texture{
private:
  GLuint texture;
  bool is_compiled = false;
  
public:
  Texture();
  Texture(TextureData data, GLint min_filter = GL_LINEAR, GLint max_filter = GL_LINEAR);
  ~Texture();

  void compile(TextureData data, GLint min_filter = GL_LINEAR, GLint max_filter = GL_LINEAR);
  void destroy();
  void bind(unsigned int socket);
  void unbind();
};
};