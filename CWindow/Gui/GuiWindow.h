#pragma once
#include "Macro.h"
#include "OpenGL/OpenGL_Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <functional>
#include <string>

namespace CW::Gui {
class GuiWindow {
public:
  std::function<void(CW::Renderer::iRenderer *renderer)> onRender;
  std::function<void()> onDestroy;

public:
  GuiWindow();
  GuiWindow(std::function<void(CW::Renderer::iRenderer *renderer)> onRender,
            std::function<void()> onDestroy = [](){});
  ~GuiWindow();
};
}; // namespace CW::Gui
