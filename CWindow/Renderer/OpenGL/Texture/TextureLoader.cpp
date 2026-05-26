#include "TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>







CW::Renderer::TextureLoader::TextureLoader(const std::string &path){
  stbi_set_flip_vertically_on_load(true);
  data.data = stbi_load(path.c_str(), &data.width, &data.height, &data.channels, 0);
  if(!data.data) return;
  
  data.format = GL_RGB;
  if (data.channels == 1) data.format = GL_RED;
  else if (data.channels == 3) data.format = GL_RGB;
  else if (data.channels == 4) data.format = GL_RGBA;
};



CW::Renderer::TextureLoader::TextureLoader(const unsigned char* buffer, size_t size){
  stbi_set_flip_vertically_on_load(true);
  
  data.data = stbi_load_from_memory(buffer, static_cast<int>(size), &data.width, &data.height, &data.channels, 0);
  if(!data.data) return;
  
  data.format = GL_RGB;
  if (data.channels == 1) data.format = GL_RED;
  else if (data.channels == 3) data.format = GL_RGB;
  else if (data.channels == 4) data.format = GL_RGBA;
}



CW::Renderer::TextureLoader::~TextureLoader() {
  stbi_image_free(data.data);
};
