#include "MeshData.h"







CW::Renderer::MeshData::MeshData() noexcept {};



CW::Renderer::MeshData::MeshData(std::vector<char>& data, const unsigned int size, const unsigned int size_of_element, const unsigned int dimension, const GLenum type) noexcept
  : data(std::move(data)), dimension(dimension), size_of_element(size_of_element), size(size), type(type){};



unsigned int CW::Renderer::MeshData::getSize() const noexcept {
  return size;
};



unsigned int CW::Renderer::MeshData::getSizeOfElement() const noexcept {
  return size_of_element;
};



unsigned int CW::Renderer::MeshData::getDimension() const noexcept {
  return dimension;
};



const char *CW::Renderer::MeshData::getRawData() const noexcept{
  return data.data();
};



GLenum CW::Renderer::MeshData::getType() const noexcept {
  return type;
};
