// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "MeshDataRecord.h"



// =============================
// ======= Constructors ========
// =============================
// core
CW::Renderer::Shared::MeshDataRecord::MeshDataRecord() noexcept {};



CW::Renderer::Shared::MeshDataRecord::MeshDataRecord(std::vector<char>& data, const unsigned int size, const unsigned int size_of_element, const unsigned int dimension, CW::Renderer::Shared::MeshDataType type) noexcept
  : data(std::move(data)), dimension(dimension), size_of_element(size_of_element), size(size), type(type){};



CW::Renderer::Shared::MeshDataRecord::~MeshDataRecord() noexcept {};



// copy
CW::Renderer::Shared::MeshDataRecord::MeshDataRecord(const MeshDataRecord& second) noexcept
  : data(second.data), dimension(second.dimension), 
    size_of_element(second.size_of_element), size(second.size),
    type(second.type) {};



CW::Renderer::Shared::MeshDataRecord& CW::Renderer::Shared::MeshDataRecord::operator=(const MeshDataRecord& second) noexcept {
  if(this == &second) return *this;
  
  data = second.data; 
  dimension = second.dimension; 
  size_of_element = second.size_of_element; 
  size = second.size;
  type = second.type;

  return *this;
};



// move
CW::Renderer::Shared::MeshDataRecord::MeshDataRecord(MeshDataRecord&& second) noexcept
  : data(std::move(second.data)), dimension(std::move(second.dimension)), 
    size_of_element(std::move(second.size_of_element)), size(std::move(second.size)),
    type(std::move(second.type)) {};



CW::Renderer::Shared::MeshDataRecord& CW::Renderer::Shared::MeshDataRecord::operator=(MeshDataRecord&& second) noexcept {
  if(this == &second) return *this;
  
  data = std::move(second.data); 
  dimension = std::move(second.dimension); 
  size_of_element = std::move(second.size_of_element); 
  size = std::move(second.size);
  type = std::move(second.type);

  return *this;
};



// =============================
// ======= Data Control ========
// =============================
unsigned int CW::Renderer::Shared::MeshDataRecord::getSize() const noexcept {
  return size;
};



unsigned int CW::Renderer::Shared::MeshDataRecord::getSizeOfElement() const noexcept {
  return size_of_element;
};



unsigned int CW::Renderer::Shared::MeshDataRecord::getDimension() const noexcept {
  return dimension;
};



const char *CW::Renderer::Shared::MeshDataRecord::getRawData() const noexcept{
  return data.data();
};



CW::Renderer::Shared::MeshDataType CW::Renderer::Shared::MeshDataRecord::getType() const noexcept {
  return type;
};
