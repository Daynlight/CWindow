#pragma once
#include "Macro.h"
#include "OpenGL/OpenGL_Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <functional>
#include <vector>

namespace CW::Gui {
class GuiWindow {
public:
  std::function<void(CW::Renderer::iRenderer *window_renderer)> render_function;
  std::function<void()> update_function;
  std::function<void()> destroy_function;

public:
  GuiWindow(std::function<void(CW::Renderer::iRenderer *window_renderer)> render_function, 
         std::function<void()> update_function = [](){},
         std::function<void()> destroy_function = [](){});
  ~GuiWindow();
};

class Gui{
private:
  CW::Renderer::iRenderer *window_renderer;
  std::function<void(std::function<void()> render_windows)> workspace;

public:
  std::vector<GuiWindow> windows;

public:
  Gui(CW::Renderer::iRenderer *window_renderer,
    std::function<void(ImGuiIO& io)> style = [](ImGuiIO& io){});
  ~Gui();
    
  void setDefaultWorkspace();
  void setWorkspace(std::function<void(std::function<void()> render_windows)> new_workspace);
  
  void render();
};
}; // namespace Gui
