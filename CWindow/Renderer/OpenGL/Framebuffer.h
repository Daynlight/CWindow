#pragma once
#include "glad/glad.h"
#include <iostream>



namespace CW::Renderer {
class Framebuffer {
private:
  GLuint fboID = 0;
  GLuint colorTextureID = 0;
  GLuint depthTextureID = 0;
  int width, height;

public:
  Framebuffer();
  Framebuffer(int width, int height);
  ~Framebuffer();

  void rescale(int width, int height);
  
  void bind() const;
  void unbind() const;

  void blitToScreen(int screenWidth, int screenHeight) const;

  GLuint getColorTexture() const { return colorTextureID; }
  GLuint getDepthTexture() const { return depthTextureID; }
};
};