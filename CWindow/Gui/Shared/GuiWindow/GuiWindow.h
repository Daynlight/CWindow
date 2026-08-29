#pragma once
#include "../../../Macro.h"

#include "../../../Renderer/OpenGL/Renderer.h"

#include "imgui.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <functional>

namespace CW::Gui {
class GuiWindow {
public:
  std::function<void(CW::Renderer::iRenderer *renderer)> onRender;
  std::function<void()> onDestroy;

public:
  GuiWindow() noexcept;
  GuiWindow(const std::function<void(CW::Renderer::iRenderer *renderer)>& onRender,
            const std::function<void()>& onDestroy = [](){}) noexcept;
  ~GuiWindow() noexcept;
};
};
