#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>

namespace CW::Renderer{
class Mesh{
private:
  GLuint VAO, VBO, EBO;
  unsigned int indices_amount = 0;

public:
  Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
  ~Mesh();
  void render();
  void bind(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
  void unbind();
};
};