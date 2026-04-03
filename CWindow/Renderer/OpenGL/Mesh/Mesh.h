#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include <vector>
#include <array>
#include <unordered_map>
#include <algorithm>
#include <cstring>
#include <stdexcept>

#include "MeshData.h"







namespace CW::Renderer{
class Mesh{
private:
  GLuint VAO, VBO, EBO = 0;

  std::unordered_map<unsigned int, CW::Renderer::MeshData> dataRegister;
  std::vector<GLuint> indices;
  
  std::array<std::vector<GLfloat>, 2> culling_box;
  bool culling_box_exists = false;

  bool is_compiled = false;

public:
  Mesh() noexcept;
  ~Mesh() noexcept;

private:
  void generateCullingBox(const std::vector<GLfloat>& data, const unsigned int dimension);

  std::vector<unsigned int> getDataRegisterLayouts() const noexcept;
  std::vector<char> generateDataBuffer(const std::vector<unsigned int>& keys, const unsigned int total_size, const unsigned int total_points) const noexcept;
  
  void genBuffers(const std::vector<char>& bufferData) noexcept;
  void setDataPositions(const std::vector<unsigned int>& keys, const unsigned int line_size) const noexcept;
  void closeBuffers() const noexcept;

public:
  void addVertices(const std::vector<GLfloat>& vertices, const unsigned int dimension = 4, const unsigned int layout = 0);
  void addIndices(const std::vector<GLuint>& indices) noexcept;

  template<typename T>
  void setData(const std::vector<T>& data, const unsigned int dimension, const unsigned int layout, const GLenum type = GL_FLOAT);
  void removeData(const unsigned int layout) noexcept;
  void clearData() noexcept;
  
  bool getCullingBoxExists() const noexcept;
  std::array<std::vector<GLfloat>, 2> getCullingBox() const noexcept;
  
  void compile();
  void destroy() noexcept;
  
  void render() noexcept ;
};
};







#include "Mesh.hpp"
