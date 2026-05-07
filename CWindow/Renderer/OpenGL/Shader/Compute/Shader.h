#pragma once 

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <string>
#include <cmath> 







namespace CW::Renderer{
class ComputeShader{
private:
  GLuint SSBO;
  GLuint compiledShader;
  std::string compute_shader = "";
  
  bool is_compiled = false;
  unsigned int data_size = 0;

public:
  ComputeShader(const std::string& compute_shader) noexcept;
  ~ComputeShader() noexcept;

  void compile() noexcept;
  void destroy() noexcept;

  template<typename T>
  void run(std::vector<T> data, unsigned int x, unsigned int y = 1, unsigned int z = 1) noexcept; 
  
  template<typename T>
  std::vector<T> get() const noexcept;

};
};







#include "Shader.hpp"