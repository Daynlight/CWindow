#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"

int main(){
  Renderer::iRenderer* window_renderer = new Renderer::Renderer();
  
  // init window and renderer
  window_renderer->createWindow();
  window_renderer->createRenderer();
  
  // compile compute shader
  window_renderer->bindComputeShader(Mandelbrot::compute);
  std::vector<float> data;
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
  Gui::Gui gui = Gui::Gui(window_renderer, &data);
  
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