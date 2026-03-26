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
  
  unsigned int vertices_dimension = 0;
  unsigned int vertices_id = 0;
  unsigned int textCords_dimension = 0;
  unsigned int textCords_id = 0;
  unsigned int color_dimension = 0;
  unsigned int color_id = 0;
  
  bool is_compiled = false;

public:
  Mesh();
  ~Mesh();

  void addVertices(std::vector<GLfloat> vertices, unsigned int dimension = 4, unsigned int layout = 0);
  void addIndicies(std::vector<GLuint> indices);
  void addColors(std::vector<GLfloat> colors, unsigned int dimension = 3, unsigned int layout = 1);
  void addTextCords(std::vector<GLfloat> textCords, unsigned int dimension = 2, unsigned int layout = 2);

  void compile();
  void destroy();
  
  void render();
};
};