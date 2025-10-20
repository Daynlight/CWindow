#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

namespace CW::Renderer{
class Mesh{
private:
  GLuint VAO, VBO, EBO;
  std::vector<GLfloat> vertices;
  std::vector<GLuint> indices;

public:
  Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
  ~Mesh();

  void compile();
  void destroy();
  
  void render();
};
};