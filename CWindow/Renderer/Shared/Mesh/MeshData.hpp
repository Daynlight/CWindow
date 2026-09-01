// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "MeshData.h"



// =============================
// ======= Data Control ========
// =============================
template <typename T>
inline void CW::Renderer::Shared::MeshData::setData(const std::vector<T>& data, const unsigned int dimension, const unsigned int layout, CW::Renderer::Shared::MeshDataType type) noexcept {
  if(dimension == 0) return;

  const unsigned int size_of_element = sizeof(T);
  const unsigned int size = data.size() * size_of_element;
  
  const char* begin = reinterpret_cast<const char*>(data.data());
  const char* end = reinterpret_cast<const char*>(data.data()) + data.size() * size_of_element;

  std::vector<char> buffer = std::vector<char>(begin, end);

  this->dataRegister[layout] = CW::Renderer::Shared::MeshDataRecord(buffer, size, size_of_element, dimension, type);

  buffer_is_ready = false;
  keys_are_ready = false;
  version++;
};
