// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include "../../Macro.h"
#include "../Interface/iRenderer.h"

#include "../Shared/Data/WindowData.h"
#include "../Shared/Data/InputData.h"
#include "../Shared/Material/PBRMaterial.h"
#include "../Shared/MeshLoader/MeshLoader.h"
#include "../Shared/Texture/TextureLoader.h"

#include "Framebuffer/Framebuffer.h"
#include "Mesh/Mesh.h"
#include "Shader/Shader/Shader.h"
#include "Shader/Compute/Shader.h"
#include "Shader/SSBO/Shader.h"
#include "Uniform/Uniform.h"
#include "Texture/Texture.h"

#include "glad/glad.h"

#include <vector>
#include <stdexcept>
#include <functional>
#include <chrono>



namespace CW::Renderer{
class Renderer : public iRenderer {
private:
  APIWindow* window;
  std::chrono::time_point<std::chrono::high_resolution_clock> last_time = std::chrono::high_resolution_clock::now();
  
  bool windowless = false;
  WindowData windowData;
  InputData inputData;
  
public:
  Renderer(bool windowless = false);
  ~Renderer();

  const WindowData* getWindowData();
  const InputData* getInputData();
  void setKeyboardBind(const std::string& action, char key);
  
  void createWindow();
  void windowLessRenderer();
  APIWindow* getWindow();
  void createRenderer();
  
  void beginFrame();
  void swapBuffer();
  void windowEvents();
  
  void setWindowMode(CW::Renderer::WindowMode mode);
  void setWindowTitle(const std::string& title);
  void setIcon(const std::string& path);
  void setVsync(bool vsync);
  void minimize(bool minimize);
  void maximize(bool maximize);
  void setPosition(int x, int y);
  void setSize(int width, int height);
  void setCursorVisibility(bool visible);
  void setCursorOn(bool on);
  void close();
};
};
