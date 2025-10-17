#pragma once
#include "Macro.h"
#include "OpenGL/OpenGL_Renderer.h"
#include "GuiWindow.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <functional>
#include <string>
#include <vector>

namespace CW::Gui {
class iGui{
public:
  virtual void setDefaultDockingWorkspace() = 0;
  virtual void render() = 0;
  virtual void addWindow(CW::Gui::GuiWindow window) = 0;
  virtual void deleteWindow(std::string name) = 0;
  virtual void deleteWindow(unsigned int index) = 0;
  virtual void setWorkspace(std::function<void(std::function<void()> render_windows)>  new_workspace) = 0;
};
}; // namespace Gui
