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
  std::vector<GLfloat> colors;
  
  bool is_compiled = false;

public:
  Mesh();
  Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices);
  ~Mesh();

  void addTextCords(std::vector<GLfloat> textCords);
  void addColors(std::vector<GLfloat> colors);

  void compile();
  void destroy();
  
  void render();
};
};