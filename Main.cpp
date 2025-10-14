#pragma once
#include "iRenderer.h"
#include "OpenGL_Renderer.h"
#include "Gui/Gui.h"

#include "Renderer/Shaders/Mandelbrot.h"

// [TODO] Uniforms
// [TODO] Vertices and edges
// [TODO] Write compute shader for parametric function, fill all pixels that needed
// [TODO] Write vertex, fragment shader that draw parametric points that was created by compute shader
// [TODO] Optimize vertex drawing only what is in viewport
// [TODO] On allow movement in window and zooming
// [TODO] On zoom and move update every idk 0.2s or sth
// [TODO] GUI with adding nodes etc
// [TODO] If nothing change dont waste gpu to generate new frame just use last one
// [TODO] DirectX
// [TODO] Vulkan

int main(){
  Renderer::iRenderer* window_renderer = new Renderer::Renderer();
  
  // init window and opengl
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