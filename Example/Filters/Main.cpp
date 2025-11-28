#include "Renderer.h"
#include "Gui.h"

#include "Shaders.h"


inline std::function<void(CW::Renderer::iRenderer *window)> settings(){
return [](CW::Renderer::iRenderer *window){
  ImGui::Text("Hello World");
};
};


int main(){
  CW::Renderer::Renderer renderer;
  CW::Gui::Gui gui(&renderer);
  gui.addWindow("Settings", settings());

  CW::Renderer::Texture texture;
  if(texture.load("image.png"))
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



  while (!renderer.getWindowData()->should_close){
    renderer.beginFrame();
    
    texture.bind();
    texture_shader.bind();
    viewport.render();
    texture_shader.unbind();
    texture.unbind();
    
    gui.render();
    
    renderer.swapBuffer();
    renderer.windowEvents();
  };
  

  return 0;
};