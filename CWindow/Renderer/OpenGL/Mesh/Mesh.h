#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>

#include "MeshData.h"







namespace CW::Renderer{
class Mesh{
private:
  GLuint VAO, VBO, EBO;

  std::unordered_map<unsigned int, CW::Renderer::MeshData> data;
  std::vector<GLuint> indices;
  
  std::array<std::vector<GLfloat>, 2> culling_box;
  bool culling_box_exists = false;

  bool is_compiled = false;

private:
  void generateCullingBox(std::vector<GLfloat> data, unsigned int dimension);
  std::vector<unsigned int> getDataKeys();
  std::vector<char> arangeData(const std::vector<unsigned int>* keys, unsigned int total_size, unsigned int total_points);

public:
  Mesh();
  ~Mesh();

  void addVertices(std::vector<GLfloat> vertices, unsigned int dimension = 4, unsigned int layout = 0);
  void addIndicies(std::vector<GLuint> indices);

  template<typename T>
  void setData(std::vector<T> data, unsigned int dimension, unsigned int layout, GLenum type);
  void removeData(unsigned int layout);
  void clearData();
  
  std::array<std::vector<GLfloat>, 2> getCullingBox() const;
  
  void compile();
  void destroy();
  
  void render();
};
};







#include "Mesh.hpp"