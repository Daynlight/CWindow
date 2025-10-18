#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"

#include <chrono>

////////////////////////// z_0.x, z_0.y, maxIter, red,   green,  blue //////////////////////////
std::vector<float> data = {0.0f,  0.0f,  500,     20.0f, 100.0f, 5.0f}; 
std::chrono::duration<float> delta_time;
bool update = true;

std::function<void(CW::Renderer::iRenderer *window_renderer)> renderSettingsWindow = [](CW::Renderer::iRenderer *renderer){
  ImGui::Begin("Settings", nullptr);

  if(delta_time.count() >= 0.0f) 
    ImGui::Text("FPS: %.f", 1.0f / delta_time.count());

  if(ImGui::InputFloat2("Z_0", &data[0], "%.3f")) update = true;
  if(ImGui::SliderFloat2("Z_0 Sidler", &data[0], -3, 3, "%.3f")) update = true;
  if(ImGui::InputFloat3("colors", &data[3], "%.3f")) update = true;
  if(ImGui::ColorPicker3("colors", &data[3])){
    for(int i = 0; i < 3; i++){
      if(data[3 + i] >1) data[3 + i] /= 255;
      data[3 + i] *= 255;
    }
    
    update = true;
  }

  int maxIter = static_cast<int>(data[2]);
  if(ImGui::InputInt("MaxIter", &maxIter)) update = true;
  data[2] = static_cast<float>(maxIter);

  if(update){
    renderer->runComputeShader(data);
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
  
  // compile compute shader
  window_renderer->bindComputeShader(Mandelbrot::compute);
  window_renderer->runComputeShader(data);
  
  // compile vertex and fragment shader
  window_renderer->bindVertexShader(Mandelbrot::vertex);
  window_renderer->bindFragmentShader(Mandelbrot::fragment);
  window_renderer->compileShaders();

  auto last_time = std::chrono::high_resolution_clock::now();
  
  // main loop
  while(window_renderer->getWindowData()->should_close){
    window_renderer->renderFrame();
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