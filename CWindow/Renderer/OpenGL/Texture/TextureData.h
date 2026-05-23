#pragma once

#include "glad/glad.h"







namespace CW::Renderer{
struct TextureData{
  GLenum format;
  int width, height, channels;
  unsigned char* data;
};
};
