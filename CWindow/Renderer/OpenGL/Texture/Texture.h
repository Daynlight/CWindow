#pragma once
#include "glad/glad.h"
#include <string>

#include "TextureLoader.h"



namespace CW::Renderer{
class Texture{
private:
  GLuint texture;
  
public:
  Texture();
  ~Texture();

  void create(TextureLoader data);
  void bind(unsigned int socket);
  void unbind();
};
};