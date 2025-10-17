#pragma once
#include "Macro.h"

#include "OpenGL/OpenGL_Renderer.h"
#include "GuiWindow/GuiWindow.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <functional>
#include <string>

namespace CW::Gui {
class iGui{
public:
  virtual void render() const = 0;
  virtual void setDefaultDockingWorkspace() noexcept = 0;
  virtual void addWindow(const std::string& name, const CW::Gui::GuiWindow& window) noexcept = 0;
  virtual void deleteWindow(const std::string& name) noexcept = 0;
  virtual void setWorkspace(std::function<void(std::function<void()> render_windows)>  new_workspace) noexcept = 0;
};
};
