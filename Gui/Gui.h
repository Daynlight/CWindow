#pragma once
#include "OpenGL/OpenGL_Renderer.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Gui
{
class Gui{
private:
  OpenGLRenderer::Renderer *renderer;
  std::vector<glm::vec2>* data;

  float new_z_0[2] = {0, 0};
  int new_max_iter = 500;
  float new_colors[3] = {50, 20, 9};
  bool update = true;

public:
  Gui(OpenGLRenderer::Renderer *renderer, std::vector<glm::vec2>* data);
  ~Gui();

  void render();
  void renderSettings();

};
  
}; // namespace Gui
