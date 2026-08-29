#pragma once
#include "glad/glad.h"

#include <string>

#include "../../Shared/Texture/TextureLoader.h"
#include "../../Shared/Texture/TextureData.h"



namespace CW::Renderer{
class Texture{
private:
  GLuint texture;
  bool is_compiled = false;
  TextureData data = TextureData();
  GLint min_filter = GL_LINEAR; 
  GLint max_filter = GL_LINEAR;

public:
  Texture();
  Texture(TextureData data, GLint min_filter = GL_LINEAR, GLint max_filter = GL_LINEAR);
  ~Texture();

  Texture(const Texture& second) noexcept;
  Texture& operator=(const Texture& second) noexcept;
  Texture(Texture&& second) noexcept;
  Texture& operator=(Texture&& second) noexcept;

  void compile(TextureData data, GLint min_filter = GL_LINEAR, GLint max_filter = GL_LINEAR);
  void compile();
  void destroy();
  void bind(unsigned int socket);
  void unbind();
};
};