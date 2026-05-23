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
  ~TextureLoader();
};
}; // namespace CW
