#include "Renderer.h"
#include "Gui.h"

#include "Shader.h"


inline std::function<void(CW::Renderer::iRenderer *window)> settings(){
return [](CW::Renderer::iRenderer *window){
  ImGui::Text("Hello World");
};
};


int main(){
  CW::Renderer::Renderer renderer;
  CW::Gui::Gui gui(&renderer);
  gui.addWindow("Settings", settings());

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


  while (!renderer.getWindowData()->should_close){
    renderer.beginFrame();
    gui.render();
    
    renderer.swapBuffer();
    renderer.windowEvents();
  };
  

  return 0;
};