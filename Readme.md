# CWindow 
## About
CWindow is cross-platform multi renderer lib for creating simple meshes, shaders, and computing on GPU.
It unify multiple renderers to simple most often used operations like, binding shaders, rendering mashes,
swapping window etc. Good to use in simple project or just learning shaders and rendering.



## Table of Content
- [CWindow](#cwindow)
  - [About](#about)
  - [Table of Content](#table-of-content)
  - [Screenshots](#screenshots)
  - [Installation](#installation)
    - [1. clone repo with submodules](#1-clone-repo-with-submodules)
    - [2. init and update submodules (if not cloned with --recursive flag)](#2-init-and-update-submodules-if-not-cloned-with---recursive-flag)
    - [3. compile via cmake with parameters for platform and renderer](#3-compile-via-cmake-with-parameters-for-platform-and-renderer)
    - [4. Run your executable](#4-run-your-executable)
  - [Configurations flags](#configurations-flags)
    - [Platforms](#platforms)
    - [Renderers](#renderers)
    - [Default and Detection](#default-and-detection)
  - [Gui Usage](#gui-usage)
    - [Initialization](#initialization)
    - [Workspace](#workspace)
      - [Info](#info)
      - [Example Workspace](#example-workspace)
    - [Adding Window](#adding-window)
      - [Info](#info-1)
      - [Example Window](#example-window)
    - [Full Example of Usage](#full-example-of-usage)
  - [WindowData](#windowdata)
    - [Info](#info-2)
    - [Data Access](#data-access)
  - [InputData](#inputdata)
    - [Info](#info-3)
    - [Data Access](#data-access-1)
  - [Implemented optimizations](#implemented-optimizations)
  - [Full Example](#full-example)
  - [Features](#features)
  - [License](#license)

## Screenshots

<table>
  <tr>
    <td align="center">
      <a href="https://raw.githubusercontent.com/Daynlight/CWindow/refs/heads/docs/docs/malgenbrota.png">
        <img src="https://raw.githubusercontent.com/Daynlight/CWindow/refs/heads/docs/docs/malgenbrota.png" /><br>
      </a>
    </td>
    <td align="center">
      <a href="https://raw.githubusercontent.com/Daynlight/CWindow/refs/heads/docs/docs/Julia.png">
        <img src="https://raw.githubusercontent.com/Daynlight/CWindow/refs/heads/docs/docs/Julia.png" /><br>
      </a>
    </td>
  </tr>
</table>



## Installation
### 1. clone repo with submodules
```bash
  git clone --recursive https://github.com/Daynlight/CWindow.git
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
### 4. Run your executable
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

### Default and Detection
1. Platform is detected in cmake
2. Default renderer is OpenGL



## Gui Usage
### Initialization
1. Initialize renderer and window
2. Initialize gui here you can provide custom gui style with ImGuiIo usage

### Workspace
#### Info
You can provide workspace
You have to provide ```std::function<void()> render_windows``` that specify place where window will be render

#### Example Workspace
```cpp
gui->setWorkspace([](std::function<void()> render_windows){
  const ImGuiViewport* viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->WorkPos);
  ImGui::SetNextWindowSize(viewport->WorkSize);

  render_windows();
});
```

### Adding Window
#### Info
1. You need to specify unique name for renderer window it is use for fast look up
2. If you want update it you need pass same name with new parameters
3. You can provide custom destruction function as second param

#### Example Window
```cpp
gui->addWindow("Example", {[](CW::Renderer::iRenderer *renderer){
  ImGui::Begin("Example", nullptr);
  ImGui::Text("Hello Gui");
  ImGui::End();
}});
```

### Full Example of Usage
```cpp
#include "OpenGL_Renderer.h"
#include "Gui.h"

int main(){
  // init window and renderer
  CW::Renderer::iRenderer* window_renderer = new CW::Renderer::Renderer();
  window_renderer->createWindow();
  window_renderer->createRenderer();
  
  // init gui and add Settings Window
  CW::Gui::iGui* gui = new CW::Gui::Gui(window_renderer);
  gui->addWindow("Example", {[](CW::Renderer::iRenderer *renderer){
    ImGui::Begin("Example", nullptr);
    ImGui::Text("Hello Gui");
    ImGui::End();
  }});
  
  // main loop
  while(window_renderer->getWindowData()->should_close){
    gui->render();
    window_renderer->windowEvents();
    window_renderer->swapBuffer();
  };

  // clean up
  delete gui;
  delete window_renderer;
  
  return 0;
}
```


<!-- 
## Renderer Usage

 -->


## WindowData
### Info
You can access InputData by ```renderer->getWindowData()```

### Data Access
* should_close
* vsync
* window_mode
* title
* is_focused
* is_minimize
* is_maximize


## InputData
### Info
You can access InputData by ```renderer->getInputData()```

### Data Access
* mouse_x;
* mouse_y;
* scroll_x;
* scroll_y;
* scroll_is_down;
* left_mouse_button_is_down;
* right_mouse_button_is_down;



<!-- ## Build-in Shaders



## Build-in Matrices -->



## Implemented optimizations
- Unordered _map for window fast look up
- On run shader compilation and reusing it
- Mesh VAO Life time control

## Full Example
```cpp
#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"

#include <chrono>

////////////////////////// z_0.x, z_0.y, maxIter, red,   green,  blue //////////////////////////
std::vector<float> data = {0.0f,  0.0f,  500,     20.0f, 100.0f, 5.0f}; 
CW::Renderer::ComputeShader* data_pass = nullptr;
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
    data_pass->run(data);
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
  data_pass = new CW::Renderer::ComputeShader(Mandelbrot::compute); 
  data_pass->run(data);
  
  // compile vertex and fragment shader
  CW::Renderer::DrawShader malgenbrot = CW::Renderer::DrawShader(Mandelbrot::vertex, Mandelbrot::fragment);

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
    malgenbrot.render();
    mesh.render();

    gui->render();
    window_renderer->windowEvents();
    window_renderer->swapBuffer();

    auto new_time = std::chrono::high_resolution_clock::now();
    delta_time = new_time - last_time;
    last_time = new_time;
  };

  // clean up
  delete data_pass;
  delete gui;
  delete window_renderer;
  
  return 0;
}
```



## Features
* Used unordered_map for window fast look up



## License
[GNU GENERAL PUBLIC LICENSE Version 2, June 1991](LICENSE)
