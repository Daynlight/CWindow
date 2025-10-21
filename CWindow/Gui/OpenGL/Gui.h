#pragma once
#include "Macro.h"

#include "../GuiWindow/GuiWindow.h"
#include "../iGui.h"
#include "OpenGL/Renderer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <functional>
#include <string>
#include <unordered_map>

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
    
  void setDefaultDockingWorkspace() noexcept override;
  void setWorkspace(std::function<void(std::function<void()> render_windows)> new_workspace) noexcept override;

  void addWindow(const std::string& name, const CW::Gui::GuiWindow& window) noexcept override;
  void deleteWindow(const std::string& name) noexcept override;
  
  void render() const override;
};
};
