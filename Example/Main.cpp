#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"
#include <chrono>


CW::Renderer::iRenderer* window = nullptr;
CW::Gui::iGui* gui = nullptr;
CW::Renderer::DrawShader* malgenbrot = nullptr;
CW::Renderer::Uniform* uniform  = nullptr;
const float scroll_sensitivity = 0.02f; 
const float sensitivity = 20.0f;
glm::vec2 last_world_pos;
glm::vec2 last_mouse_pos;




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
  (*uniform)["world_pos"]->set<glm::vec2>({0.0f, 0.0f});
  (*uniform)["zoom"]->set<float>(1.0f);

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

    if(window->getInputData()->right_mouse_button_is_down){
      (*uniform)["z"]->set<glm::vec2>({
        2 * (window->getWindowData()->width / 2 - window->getInputData()->mouse_x) / window->getWindowData()->width, 
        2 * (window->getWindowData()->height / 2 - window->getInputData()->mouse_y) / window->getWindowData()->height
      });
    }

    if(window->getInputData()->scroll_is_down){
      (*uniform)["world_pos"]->set<glm::vec2>({
        last_world_pos.x + (window->getInputData()->mouse_x - last_mouse_pos.x) / (*uniform)["zoom"]->get<float>(), 
        last_world_pos.y + (window->getInputData()->mouse_y - last_mouse_pos.y) / (*uniform)["zoom"]->get<float>()
      });
    }
    else{
      last_world_pos = (*uniform)["world_pos"]->get<glm::vec2>();
      last_mouse_pos = {window->getInputData()->mouse_x, window->getInputData()->mouse_y};
    };


    float zoom = (*uniform)["zoom"]->get<float>();
    zoom += window->getInputData()->scroll_y * scroll_sensitivity * zoom;
    zoom = glm::clamp(zoom, 0.0001f, 10.0f);
    (*uniform)["zoom"]->set<float>(zoom);

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