#include "Mesh.h"







template <typename T>
inline void CW::Renderer::Mesh::setData(const std::vector<T>& data, const unsigned int dimension, const unsigned int layout, const GLenum type){
  if(dimension == 0) throw std::runtime_error("Data dimension == 0, no data to add");

  const unsigned int size_of_element = sizeof(T);
  const unsigned int size = data.size() * size_of_element;
  
  const char* begin = reinterpret_cast<const char*>(data.data());
  const char* end = reinterpret_cast<const char*>(data.data()) + data.size() * size_of_element;

  std::vector<char> buffer = std::vector<char>(begin, end);

  this->dataRegister[layout] = CW::Renderer::MeshData(buffer, size, size_of_element, dimension, type);
  
  is_compiled = false;
};
