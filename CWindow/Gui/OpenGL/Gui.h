#pragma once
#include "Macro.h"
#include "OpenGL/OpenGL_Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace CW::Gui
{
class Gui{
private:
  CW::Renderer::iRenderer *window_renderer;
  std::vector<float>* data;

  float new_z_0[2] = {0, 0};
  int new_max_iter = 500;
  float new_colors[3] = {50, 20, 9};
  bool update = true;

public:
  Gui(CW::Renderer::iRenderer *window_renderer, std::vector<float>* data);
  ~Gui();

  void render();
  void renderSettings();
};
}; // namespace Gui
