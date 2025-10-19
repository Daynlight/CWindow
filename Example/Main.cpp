#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"
#include <chrono>


CW::Renderer::iRenderer* window = nullptr;
CW::Gui::iGui* gui = nullptr;
CW::Renderer::DrawShader* malgenbrot = nullptr;
CW::Renderer::Uniform* uniform  = nullptr;




std::function<void(CW::Renderer::iRenderer *window)> renderSettingsWindow = [](CW::Renderer::iRenderer *window){
  glm::vec2 z = (*uniform)["z"]->get<glm::vec2>(); 
  int maxIter = (*uniform)["maxIter"]->get<int>();
  glm::vec3 colors = (*uniform)["colors"]->get<glm::vec3>();

  ImGui::Begin("Settings", nullptr);

  if(window->getWindowData()->delta_time >= 0.0f) 
    ImGui::Text("FPS: %.f", 1.0f / window->getWindowData()->delta_time);

  ImGui::InputFloat2("Z_0", &z[0], "%.3f");
  ImGui::SliderFloat2("Z_0 Sidler", &z[0], -3, 3, "%.3f");

  ImGui::InputFloat3("colors", &colors[0], "%.3f");
  if(ImGui::ColorPicker3("colors", &colors[0])){
    for(int i = 0; i < 3; i++){
      if(colors[0] >1) colors[0] /= 255;
      colors[0] *= 255;
    }
  }

  ImGui::InputInt("MaxIter", &maxIter);
  ImGui::End();

  (*uniform)["z"]->set<glm::vec2>(z);
  (*uniform)["maxIter"]->set<int>(maxIter);
  (*uniform)["colors"]->set<glm::vec3>(colors);
};





int main(){
  CW::Renderer::iRenderer* window = new CW::Renderer::Renderer();
  
  // init window and renderer
  window->createWindow();
  window->createRenderer();
  window->setVsync(0);
  window->setWindowTitle("Malgenbrota and Julia");
  
  // init gui and add Settings Window
  gui = new CW::Gui::Gui(window);
  gui->addWindow("Settings", renderSettingsWindow);
  
  // create uniform and malgenbrota shader
  uniform = new CW::Renderer::Uniform();
  malgenbrot = new CW::Renderer::DrawShader(Mandelbrot::vertex, Mandelbrot::fragment);
  malgenbrot->getUniforms().emplace_back(uniform);

  // uniform default values
  (*uniform)["z"]->set<glm::vec2>({0.0f, 0.5f});
  (*uniform)["maxIter"]->set<int>(500);
  (*uniform)["colors"]->set<glm::vec3>({20.0f, 100.0f, 5.0f});

  // create viewport mesh
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
  
  // main loop
  while(window->getWindowData()->should_close){
    window->beginFrame();

    malgenbrot->bind();
    viewport.render();
    malgenbrot->unbind();

    gui->render();
    window->windowEvents();
    window->swapBuffer();
  };

  // clean up
  delete uniform;
  delete malgenbrot;
  delete gui;
  delete window;
  
  return 0;
}