# CWindow 
## About
CWindow is cross-platform multi renderer lib for creating simple meshes, shaders, and computing on GPU.
It unify multiple renderers to simple most often used operations like, binding shaders, rendering mashes,
swapping window etc. Good to use in simple project or just learning shaders and rendering.

## Screenshots

<div style="float: left; width: calc(50% - 20px); margin-right: 20px;">
  <img src="docs/malgenbrota.png" alt="Malgenbrota" style="width: 100%;" />
</div>
<div style="float: left; width: calc(50% - 20px);">
  <img src="docs/Julia.png" alt="Julia" style="width: 100%;" />
</div>
<div style="clear: both;"></div>

## Installation
### 1. clone repo with submodules
```bash
  git --recursive clone https://github.com/Daynlight/CWindow.git
```
### 2. init and update submodules (if not cloned with --recursive flag)
```bash
  git submodule init
  git submodule update
```
### 3. compile via cmake with parameters for platform and renderer
```bash
  mkdir build/
  cd build/
  cmake .. -DRENDERER="DIRECTX" -DPLATFORM="WIN32"
```
### 4. Run it your executable
```bash
  ./CWindow.exe
```

## Configurations flags
### Platforms
1. WIN32 - windows platform
2. UNIX - linux platform
3. MACOS - mac platform

### Renderers
1. OPENGL - OpenGL (glad 4.3, glfw cross-platform)
2. DIRECTX - directx 12 (windows only)
3. VULKAN - vulkan renderer (cross-platform)
4. METAL - macos renderer (macos only)


## Usage

## iRenderer interface

## Build-in Shaders

## Build-in Matrices

## Implemented optimizations

## Example
```cpp
#include "iRenderer.h"
#include "OpenGL_Renderer.h"
#include "Gui/Gui.h"

#include "Renderer/Shaders/Mandelbrot.h"

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
```

## Features

## License
