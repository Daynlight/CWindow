// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../../Shared/Uniform/UniformData.h"



namespace CW::Renderer{
class Uniform{
private:
  GLuint UBO;
  CW::Renderer::Shared::UniformData uniform_data;
  
public:
  Uniform();
  ~Uniform();

  void compile();
  void destroy();

  void bind(GLuint& shader);

  void setUniformData(const CW::Renderer::Shared::UniformData& data) noexcept;
  CW::Renderer::Shared::UniformData getUniformData() noexcept;
};
};
