#include "TextureLoader.h"


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>



CW::Renderer::TextureLoader::TextureLoader(const std::string& path)
{
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  if(!data) return;
  
  format = GL_RGB;
  if (channels == 1) format = GL_RED;
  else if (channels == 3) format = GL_RGB;
  else if (channels == 4) format = GL_RGBA;
};


CW::Renderer::TextureLoader::~TextureLoader() {
  stbi_image_free(data);
};