#include "Renderer.h"
#include "Shaders.h"


int main(){
  CW::Renderer::Renderer window;

  CW::Renderer::Texture texture;
  if(texture.load("../Assets/image.png"))
    return -1;

  CW::Renderer::Mesh viewport = CW::Renderer::Mesh(
  {
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f,  1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
  }, 
  {
    0, 1, 2,
    1, 3, 2
  });

  viewport.addTextCords(
  {
    0.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  });

  CW::Renderer::DrawShader texture_shader(Texture::vertex, Texture::fragment);
  CW::Renderer::Uniform uniform;
  texture_shader.getUniforms().emplace_back(&uniform);
  uniform["uTexture"]->set<int>(0);


  while(!window.getWindowData()->should_close){
    window.beginFrame();

    texture.bind();
    texture_shader.bind();
    viewport.render();
    texture_shader.unbind();
    texture.unbind();

    window.swapBuffer();
    window.windowEvents();
  };

  return 0;
}