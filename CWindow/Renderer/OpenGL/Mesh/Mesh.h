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
  std::vector<GLfloat> textCords;
  bool is_compiled = false;

public:
  Mesh();
  Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
  ~Mesh();

  void addTextCords(std::vector<GLfloat> textCords);

  void compile();
  void destroy();
  
  void render();
};
};