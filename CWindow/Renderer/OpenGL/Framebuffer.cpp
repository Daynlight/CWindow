#include "Framebuffer.h"



CW::Renderer::Framebuffer::Framebuffer(){

};



CW::Renderer::Framebuffer::Framebuffer(int width, int height) : width(width), height(height) {
  rescale(width, height);
};



CW::Renderer::Framebuffer::~Framebuffer() {
  if(fboID != 0){
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &colorTextureID);
    glDeleteTextures(1, &depthTextureID);
  };
};



void CW::Renderer::Framebuffer::rescale(int w, int h) {
  this->width = w;
  this->height = h;

  if (fboID != 0) {
    glDeleteFramebuffers(1, &fboID);
    glDeleteTextures(1, &colorTextureID);
    glDeleteTextures(1, &depthTextureID);
  };

  glGenFramebuffers(1, &fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);

  glGenTextures(1, &colorTextureID);
  glBindTexture(GL_TEXTURE_2D, colorTextureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTextureID, 0);

  glGenTextures(1, &depthTextureID);
  glBindTexture(GL_TEXTURE_2D, depthTextureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureID, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
  };

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};



void CW::Renderer::Framebuffer::bind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, fboID);
  glViewport(0, 0, width, height);
};



void CW::Renderer::Framebuffer::unbind() const {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};



void CW::Renderer::Framebuffer::blitToScreen(int screenWidth, int screenHeight) const {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, fboID);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 

  glBlitFramebuffer(
    0, 0, this->width, this->height,
    0, 0, screenWidth, screenHeight,
    GL_COLOR_BUFFER_BIT,
    GL_LINEAR
  );

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
};