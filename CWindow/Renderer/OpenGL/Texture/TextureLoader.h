#pragma once
#include "glad/glad.h"

#include <string>




namespace CW::Renderer{
class TextureLoader{
public:
  GLenum format;
  int width, height, channels;
  unsigned char* data;

public:
  TextureLoader(const std::string& path);
  ~TextureLoader();
};
}; // namespace CW
