#pragma once
#include "glad/glad.h"

#include <vector>







namespace CW::Renderer{
class MeshData{
private:
  std::vector<char> data;
  unsigned int dimension = 0;
  unsigned int size_of_element = 0;
  unsigned int size = 0;
  GLenum type = GL_FLOAT;

public:
  MeshData() noexcept;
  MeshData(std::vector<char>& data, const unsigned int size, const unsigned int size_of_element, const unsigned int dimension, const GLenum type) noexcept;

  unsigned int getSize() const noexcept;
  unsigned int getSizeOfElement() const noexcept;
  unsigned int getDimension() const noexcept;
  const char* getRawData() const noexcept;
  GLenum getType() const noexcept;

};
};
