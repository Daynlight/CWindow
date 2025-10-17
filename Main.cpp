#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"

////////////////////////// z_0.x, z_0.y, maxIter, red,   green,  blue //////////////////////////
std::vector<float> data = {0.0f,  0.0f,  500,     20.0f, 100.0f, 5.0f}; 
float new_z_0[2] = {0, 0};
int new_max_iter = 500;
float new_colors[3] = {50, 20, 9};
bool update = true;

std::function<void(CW::Renderer::iRenderer *window_renderer)> renderSettingsWindow = [](CW::Renderer::iRenderer *renderer){
  ImGui::Begin("Settings", nullptr);

  if(ImGui::InputFloat2("Z_0", new_z_0, "%.3f")) update = true;
  if(ImGui::SliderFloat2("Z_0 Sidler", new_z_0, -3, 3, "%.3f")) update = true;
  if(ImGui::InputFloat3("colors", new_colors, "%.3f")) update = true;
  if(ImGui::ColorPicker3("colors", new_colors)){
    for(int i = 0; i < 3; i++){
      if(new_colors[i] >1) new_colors[i] /= 255;
      new_colors[i] *= 255;
    }
    
    update = true;
  }
  if(ImGui::InputInt("MaxIter", &new_max_iter)) update = true;

  if(update){
    data[0] = new_z_0[0];
    data[1] = new_z_0[1];
    data[2] = new_max_iter;
    data[3] = new_colors[0];
    data[4] = new_colors[1];
    data[5] = new_colors[2];

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
  
  // init gui and add Settings Window
  CW::Gui::iGui* gui = new CW::Gui::Gui(window_renderer);
  gui->addWindow({"Settings", renderSettingsWindow});
  
  // compile compute shader
  window_renderer->bindComputeShader(Mandelbrot::compute);
  window_renderer->runComputeShader(data);
  
  // compile vertex and fragment shader
  window_renderer->bindVertexShader(Mandelbrot::vertex);
  window_renderer->bindFragmentShader(Mandelbrot::fragment);
  window_renderer->compileShaders();
  
  // main loop
  while(window_renderer->isRunning()){
    window_renderer->renderFrame();
    gui->render();
    window_renderer->windowEvents();
    window_renderer->swapBuffer();
  };

  // clean up
  delete gui;
  delete window_renderer;
  return 0;
}