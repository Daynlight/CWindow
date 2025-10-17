#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"

std::vector<float> data;
float new_z_0[2] = {0, 0};
int new_max_iter = 500;
float new_colors[3] = {50, 20, 9};
bool update = true;

std::function<void(CW::Renderer::iRenderer *window_renderer)> renderSettings = [](CW::Renderer::iRenderer *window_renderer){
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

    window_renderer->runComputeShader(data);
    update = false;
  }

  ImGui::End();
};






int main(){
  CW::Renderer::iRenderer* window_renderer = new CW::Renderer::Renderer();
  
  // init window and renderer
  window_renderer->createWindow();
  window_renderer->createRenderer();
  
  // compile compute shader
  window_renderer->bindComputeShader(Mandelbrot::compute);
  data.emplace_back(0.0f);    // z_0
  data.emplace_back(0.0f);    // z_0
  data.emplace_back(500);     // maxIter
  data.emplace_back(20.0f);   // red
  data.emplace_back(100.0f);  // green
  data.emplace_back(5.0f);    // blue
  window_renderer->runComputeShader(data);
  
  // compile vertex and fragment shader
  window_renderer->bindVertexShader(Mandelbrot::vertex);
  window_renderer->bindFragmentShader(Mandelbrot::fragment);
  window_renderer->compileShaders();
  
  // init gui
  CW::Gui::Gui gui = CW::Gui::Gui(window_renderer);
  gui.windows.emplace_back(renderSettings);
  
  // main loop
  while(window_renderer->isRunning()){
    window_renderer->renderFrame();
    gui.render();
    window_renderer->windowEvents();
    window_renderer->swapBuffer();
  };

  delete window_renderer;

  return 0;
}