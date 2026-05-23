#pragma once 

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <cmath> 







namespace CW::Renderer{
class GPUStore{
private:
  GLuint SSBO;
  
  bool is_created = false;
  unsigned int data_size = 0;
  GLuint slot = 0;

public:
  GPUStore() noexcept;
  ~GPUStore() noexcept;

  void create() noexcept;
  void destroy() noexcept;

  template<typename T>
  void set(const std::vector<T>& data) noexcept; 
  
  void bind(GLuint socket = 0) noexcept;
  void unbind() noexcept;
};
};







#include "Shader.hpp"