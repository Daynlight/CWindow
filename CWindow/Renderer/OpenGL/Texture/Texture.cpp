#include "Texture.h"







CW::Renderer::Texture::Texture() : is_compiled(false) {};



CW::Renderer::Texture::Texture(TextureData data, GLint min_filter, GLint max_filter)
: is_compiled(false),
  data(data),
  min_filter(min_filter), 
  max_filter(max_filter){
  compile();
};



CW::Renderer::Texture::~Texture() {
  destroy();
};



CW::Renderer::Texture::Texture(const Texture &second) noexcept
  : texture(second.texture),
    is_compiled(second.is_compiled),
    data(second.data),
    min_filter(second.min_filter), 
    max_filter(second.max_filter){};



Texture &CW::Renderer::Texture::operator=(const Texture &second) noexcept{
  if(this == &second) return *this;

  texture = second.texture;
  is_compiled = second.is_compiled;
  data = second.data;
  min_filter = second.min_filter; 
  max_filter = second.max_filter;

  return *this;
};



CW::Renderer::Texture::Texture(Texture &&second) noexcept 
  : texture(std::move(second.texture)),
    is_compiled(std::move(second.is_compiled)),
    data(std::move(second.data)),
    min_filter(std::move(second.min_filter)), 
    max_filter(std::move(second.max_filter)){
  second.texture = 0;
  second.data = TextureData();
  second.is_compiled = false;
  second.min_filter = GL_LINEAR; 
  second.max_filter = GL_LINEAR
};



Texture &CW::Renderer::Texture::operator=(Texture &&second) noexcept{
  if(this == &second) return *this;

  texture = std::move(second.texture);
  is_compiled = std::move(second.is_compiled);
  data = std::move(second.data);
  min_filter = std::move(second.min_filter); 
  max_filter = std::move(second.max_filter);

  second.texture = 0;
  second.data = TextureData();
  second.is_compiled = false;
  second.min_filter = GL_LINEAR; 
  second.max_filter = GL_LINEAR

  return *this;
};



void CW::Renderer::Texture::compile(TextureData data, GLint min_filter, GLint max_filter) {
  this->data = data;
  this->min_filter = min_filter; 
  this->max_filter = max_filter;

  compile();
};




void CW::Renderer::Texture::compile() {

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, max_filter);

  glTexImage2D(GL_TEXTURE_2D, 0, data.format, data.width, data.height, 0, data.format, GL_UNSIGNED_BYTE, data.data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, 0); 
  
  is_compiled = true;
};



void CW::Renderer::Texture::destroy(){
  if(is_compiled)
    glDeleteTextures(1, &texture);
  
  is_compiled = false;
}



void CW::Renderer::Texture::bind(unsigned int socket) {
  if(!is_compiled) 
    return;

  glActiveTexture(GL_TEXTURE0 + socket);
  glBindTexture(GL_TEXTURE_2D, texture);
};



void CW::Renderer::Texture::unbind() {
  glBindTexture(GL_TEXTURE_2D, 0);
};
