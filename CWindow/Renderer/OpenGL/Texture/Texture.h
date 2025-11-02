#pragma once

#include "glad/glad.h"
#include <string>

namespace CW::Renderer{
class Texture{
private:
  GLuint texture;
  
public:
  Texture();
  ~Texture();

  bool load(const std::string& path);
  void bind();
  void unbind();
};
};