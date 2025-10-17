#pragma once
#include "Macro.h"
#include "../GuiWindow.h"
#include "../iGui.h"
#include "OpenGL/OpenGL_Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <functional>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <vector>

namespace CW::Gui {
class Gui : public iGui{
private:
  CW::Renderer::iRenderer *renderer;
  std::function<void(std::function<void()> render_windows)> workspace;
  std::unordered_map<std::string, CW::Gui::GuiWindow> windows;

public:
  Gui(CW::Renderer::iRenderer *renderer,
    std::function<void(ImGuiIO& io)> style = [](ImGuiIO& io){});
  ~Gui();
    
  void setDefaultDockingWorkspace();
  void setWorkspace(std::function<void(std::function<void()> render_windows)> new_workspace);

  void addWindow(std::string name, CW::Gui::GuiWindow window);
  void deleteWindow(std::string name);
  
  void render();
};
}; // namespace Gui
