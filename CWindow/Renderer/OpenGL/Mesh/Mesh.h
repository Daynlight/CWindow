#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "MeshData.h"


namespace CW::Renderer{
class Mesh{
private:
  GLuint VAO, VBO, EBO;
  std::vector<GLuint> indices;
  std::unordered_map<unsigned int, CW::Renderer::MeshData> data;

  bool is_compiled = false;

public:
  Mesh();
  ~Mesh();

  void addVertices(std::vector<GLfloat> vertices, unsigned int dimension = 4, unsigned int layout = 0);
  void addIndicies(std::vector<GLuint> indices);

  template<typename T>
  void addData(std::vector<T> data, unsigned int dimension, unsigned int layout, GLenum type);

  void compile();
  void destroy();
  
  void render();
};
};


#include "Mesh.hpp"