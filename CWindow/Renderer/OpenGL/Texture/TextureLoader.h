#pragma once
#include "glad/glad.h"

#include <string>

#include "TextureData.h"







namespace CW::Renderer{
class TextureLoader{
public:
  TextureData data;

public:
  TextureLoader(const std::string& path);
  TextureLoader(const unsigned char* buffer, size_t size);
  ~TextureLoader();
};
}; // namespace CW
