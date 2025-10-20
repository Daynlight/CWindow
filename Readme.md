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
    - [3 Add CWindow to your cmake project](#3-add-cwindow-to-your-cmake-project)
    - [4.1 compile via cmake](#41-compile-via-cmake)
    - [4.2 compile via cmake with parameters for platform and renderer](#42-compile-via-cmake-with-parameters-for-platform-and-renderer)
    - [5 Run your executable](#5-run-your-executable)
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
  - [Renderer Usage](#renderer-usage)
    - [Info](#info-2)
    - [Editing Window](#editing-window)
    - [Window loop](#window-loop)
    - [Getting window ref](#getting-window-ref)
  - [WindowData](#windowdata)
    - [Info](#info-3)
    - [Data Access](#data-access)
  - [InputData](#inputdata)
    - [Info](#info-4)
    - [Data Access](#data-access-1)
  - [Uniform](#uniform)
  - [DrawShader](#drawshader)
  - [ComputeShader](#computeshader)
  - [Mesh](#mesh)
    - [Info](#info-5)
    - [Data Stored](#data-stored)
    - [Mesh control](#mesh-control)
    - [Render](#render)
  - [Implemented optimizations](#implemented-optimizations)
  - [Full Example](#full-example)
  - [Features](#features)
  - [Libraries](#libraries)
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
### 3 Add CWindow to your cmake project
```cmake
  cmake_minimum_required(VERSION 3.15)

  project(Example LANGUAGES CXX C)

  add_subdirectory(CWindow/CWindow)

  set(src "Main.cpp")
  set(headers "Mandelbrot.h")

  add_executable(Example ${src})
  target_link_libraries(Example CWindow)
```
### 4.1 compile via cmake
```bash
  mkdir build/
  cd build/
  cmake ..
```
### 4.2 compile via cmake with parameters for platform and renderer
```bash
  mkdir build/
  cd build/
  cmake .. -DRENDERER="DIRECTX" -DPLATFORM="WIN32"
```
### 5 Run your executable
```bash
  ./Example.exe
```



## Configurations flags
### Platforms
1. WIN32 - windows platform
2. UNIX - linux platform

### Renderers
1. OPENGL - OpenGL (glad 4.3, glfw cross-platform)
<!-- 2. DIRECTX - directx 12 (windows only)
3. VULKAN - vulkan renderer (cross-platform) -->

### Default and Detection
1. Platform is detected in cmake
2. Default renderer is OpenGL



## Gui Usage
### Initialization
1. Initialize renderer and window
2. Initialize gui. Here you can provide custom gui style with ImGuiIo parameter

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
1. You need to specify unique name for renderer window. It is used for fast look up
2. If you want update it you need add window with same name
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



## Renderer Usage
### Info 
1. Platform is detected automatically
2. When Renderer Initialized auto window creation and renderer

### Editing Window
1. You can edit, name, size, position, mode by functions in renderer class

* setWindowMode(CW::Renderer::WindowMode mode);
* setWindowTitle(const std::string& title);
* setVsync(bool vsync);
* minimizedSwitch();
* maximizeSwitch();

### Window loop
* beginFrame();   -- starting new frame
* swapBuffer();   -- swapping window frame
* windowEvents(); -- is used to update WindowData and InputData

### Getting window ref
* APIWindow* getWindow(); -- where APIWindow is your Renderer Window



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
* delta_time



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



## Uniform



## DrawShader



## ComputeShader



## Mesh
### Info
1. Mesh store data for rendering
2. When some data is not provided then automatically didn't add to shader
3. vertices and indices are required
4. automatically compiled when used and don't compiled before


### Data Stored
1. vertices (vec3)
2. indices (int)
<!-- 3. normals (vec3) -->

### Mesh control
* compile();
* destroy(); 
* render();   -- used for rendering on viewport used with DrawShader

### Render
```cpp
  malgenbrot->bind();   -- shader
  viewport.render();    -- mesh
  malgenbrot->unbind(); -- shader
```



## Implemented optimizations
- Unordered _map for window fast look up
- On run shader compilation and reusing it
- Mesh and Shader lifetime control by ```compile``` and ```destroy```
- Mesh and Shader auto compile when used
- Storing Data every ```windowEvent()``` instead of running all api commands 



## Full Example
```cpp
#include "OpenGL_Renderer.h"
#include "Gui.h"
#include "Mandelbrot.h"


CW::Renderer::iRenderer* window = nullptr;
CW::Gui::iGui* gui = nullptr;
CW::Renderer::DrawShader* malgenbrot = nullptr;
CW::Renderer::Uniform* uniform  = nullptr;


const float scroll_sensitivity = 0.02f; 
const float sensitivity = 20.0f;
glm::vec2 last_world_pos;
glm::vec2 last_mouse_pos;
bool animation = false;
const float zoom_speed = 0.005;
float current_zoom_speed = 0.005;



std::function<void(CW::Renderer::iRenderer *window)> renderSettingsWindow = [](CW::Renderer::iRenderer *window){
  glm::vec2 z = (*uniform)["z"]->get<glm::vec2>(); 
  int maxIter = (*uniform)["maxIter"]->get<int>();
  glm::vec3 colors = (*uniform)["colors"]->get<glm::vec3>();
  colors /= 255;

  ImGui::Begin("Settings", nullptr);

  if(window->getWindowData()->delta_time >= 0.0f) 
    ImGui::Text("FPS: %.f", 1.0f / window->getWindowData()->delta_time);

  ImGui::InputFloat2("Z_0", &z[0], "%.3f");
  ImGui::SliderFloat2("Z_0 Sidler", &z[0], -3, 3, "%.3f");

  ImGui::InputFloat3("colors", &colors[0], "%.3f");
  ImGui::ColorPicker3("colors", &colors[0]);

  ImGui::InputInt("MaxIter", &maxIter);
  
  if(ImGui::Button("Animation")) 
    animation = !animation;

  ImGui::End();

  if(animation){
    if((*uniform)["zoom"]->get<float>() < 0.002) 
      current_zoom_speed = -1 * (zoom_speed);

    if((*uniform)["zoom"]->get<float>() > 3)
      current_zoom_speed = (zoom_speed);
     
    (*uniform)["zoom"]->set<float>((*uniform)["zoom"]->get<float>() - (*uniform)["zoom"]->get<float>() * current_zoom_speed);
  }

  (*uniform)["z"]->set<glm::vec2>(z);
  (*uniform)["maxIter"]->set<int>(maxIter);
  (*uniform)["colors"]->set<glm::vec3>(colors * 255.0f);
};



int main(){
  CW::Renderer::iRenderer* window = new CW::Renderer::Renderer();
  
  // init window and renderer
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
  (*uniform)["z"]->set<glm::vec2>({0.394f, 0.355f});
  (*uniform)["maxIter"]->set<int>(500);
  (*uniform)["colors"]->set<glm::vec3>({20.0f, 100.0f, 5.0f});
  (*uniform)["world_pos"]->set<glm::vec2>({20.0f, 0.0f});
  (*uniform)["zoom"]->set<float>(3.0f);
  (*uniform)["window_ratio"]->set<glm::vec2>({
    window->getWindowData()->width,
    window->getWindowData()->height
  });

  
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

  (*uniform)["window_ratio"]->set<glm::vec2>({
    window->getWindowData()->width,
    window->getWindowData()->height
  });


    if(window->getInputData()->right_mouse_button_is_down){
      (*uniform)["z"]->set<glm::vec2>({
        3 * (window->getWindowData()->width / 2 - window->getInputData()->mouse_x) / window->getWindowData()->width, 
        3 * (window->getWindowData()->height / 2 - window->getInputData()->mouse_y) / window->getWindowData()->height
      });
    }

    if(window->getInputData()->scroll_is_down){
      (*uniform)["world_pos"]->set<glm::vec2>({
        last_world_pos.x - (window->getInputData()->mouse_x - last_mouse_pos.x) * (*uniform)["zoom"]->get<float>(),
        last_world_pos.y + (window->getInputData()->mouse_y - last_mouse_pos.y) * (*uniform)["zoom"]->get<float>()
      });
    }
    else{
      last_world_pos = (*uniform)["world_pos"]->get<glm::vec2>();
      last_mouse_pos = {window->getInputData()->mouse_x, window->getInputData()->mouse_y};
    };


    float zoom = (*uniform)["zoom"]->get<float>();
    zoom += window->getInputData()->scroll_y * scroll_sensitivity * zoom;
    zoom = glm::clamp(zoom, 0.000001f, 10.0f);
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
```



## Features
* Automatic Uniform parameters binding to shader
* Autocompletion when Mesh, Uniform or Shader used
* Platform detection
* Creating window and renderer
* Creating Modular Shader
* Creating Modular Meshes
* Creating Modular Uniform list with references by name
* Binding Uniforms to shader and using as ```uniform vec2 name```
* Compute Shader form computation on gpu
* Editing Window
* Getting user input
* Getting window parameters and store it at once



## Libraries
* [glfw](https://github.com/glfw/glfw)
* [glad](https://glad.dav1d.de/)
* [imgui](https://github.com/ocornut/imgui/tree/docking)
* [glm](https://github.com/g-truc/glm)



## License
[GNU GENERAL PUBLIC LICENSE Version 2, June 1991](LICENSE)
