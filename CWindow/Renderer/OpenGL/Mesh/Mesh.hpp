#include "Mesh.h"



template <typename T>
inline void CW::Renderer::Mesh::setData(std::vector<T> data, unsigned int dimension, unsigned int layout, GLenum type){
  const char* begin = reinterpret_cast<const char*>(data.data());
  const char* end   = begin + data.size() * sizeof(T);

  std::vector<char> buffer = std::vector<char>(begin, end);
  unsigned int size_of_element = sizeof(T);

  this->data[layout] = CW::Renderer::MeshData(buffer, dimension, size_of_element, type);
  is_compiled = false;
};