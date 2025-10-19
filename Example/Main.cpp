#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"
#include <chrono>

CW::Renderer::Uniform uniform;
std::chrono::duration<float> delta_time;
bool update = true;

std::function<void(CW::Renderer::iRenderer *window_renderer)> renderSettingsWindow = [](CW::Renderer::iRenderer *renderer){
  ImGui::Begin("Settings", nullptr);

  if(delta_time.count() >= 0.0f) 
    ImGui::Text("FPS: %.f", 1.0f / delta_time.count());

  glm::vec2 z = uniform["z"]->get<glm::vec2>(); 
  int maxIter = uniform["maxIter"]->get<int>();
  glm::vec3 colors = uniform["colors"]->get<glm::vec3>();

  if(ImGui::InputFloat2("Z_0", &z[0], "%.3f")) update = true;
  if(ImGui::SliderFloat2("Z_0 Sidler", &z[0], -3, 3, "%.3f")) update = true;
  if(ImGui::InputFloat3("colors", &colors[0], "%.3f")) update = true;
  if(ImGui::ColorPicker3("colors", &colors[0])){
    for(int i = 0; i < 3; i++){
      if(colors[0] >1) colors[0] /= 255;
      colors[0] *= 255;
    }
    
    update = true;
  }

  if(ImGui::InputInt("MaxIter", &maxIter)) update = true;

  if(update){
    uniform["z"]->set<glm::vec2>(z);
    uniform["maxIter"]->set<int>(maxIter);
    uniform["colors"]->set<glm::vec3>(colors);
    uniform.compile();
    update = false;
  }

  ImGui::End();
};









int main(){
  CW::Renderer::iRenderer* window_renderer = new CW::Renderer::Renderer();
  
  // init window and renderer
  window_renderer->createWindow();
  window_renderer->createRenderer();
  window_renderer->setVsync(0);
  window_renderer->setWindowTitle("Malgenbrota and Julia");
  
  // init gui and add Settings Window
  CW::Gui::iGui* gui = new CW::Gui::Gui(window_renderer);
  gui->addWindow("Settings", renderSettingsWindow);
  
  // compile drawShader
  uniform["z"]->set<glm::vec2>({0.0f, 0.5f});
  uniform["maxIter"]->set<int>(500);
  uniform["colors"]->set<glm::vec3>({20.0f, 100.0f, 5.0f});
  uniform.compile();
  CW::Renderer::DrawShader malgenbrot = CW::Renderer::DrawShader(Mandelbrot::vertex, Mandelbrot::fragment, &uniform);


  // create viewport mesh
  std::vector<GLfloat> vertices = {
    // Positions
    -1.0f,  1.0f,  // Top-left
    -1.0f, -1.0f,  // Bottom-left
    1.0f,  1.0f,  // Top-right
    1.0f, -1.0f,  // Bottom-right
  };
  std::vector<GLuint> indices = {
    0, 1, 2,  // First triangle
    1, 3, 2   // Second triangle
  };
  CW::Renderer::Mesh mesh = CW::Renderer::Mesh(vertices, indices);


  auto last_time = std::chrono::high_resolution_clock::now();
  
  // main loop
  while(window_renderer->getWindowData()->should_close){
    window_renderer->beginFrame();
    malgenbrot.bind();
    mesh.render();
    malgenbrot.unbind();

    gui->render();
    window_renderer->windowEvents();
    window_renderer->swapBuffer();

    auto new_time = std::chrono::high_resolution_clock::now();
    delta_time = new_time - last_time;
    last_time = new_time;
  };

  // clean up
  delete gui;
  delete window_renderer;
  
  return 0;
}