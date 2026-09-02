// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "UniformDataRecord.h"



// =============================
// ======= Constructors ========
// =============================
// core
inline CW::Renderer::Shared::UniformDataRecord::UniformDataRecord() noexcept {};



inline CW::Renderer::Shared::UniformDataRecord::~UniformDataRecord() noexcept {};


// copy
inline CW::Renderer::Shared::UniformDataRecord::UniformDataRecord(const UniformDataRecord& second) noexcept 
  : value(second.value), size(second.size), type(second.type) {};



inline CW::Renderer::Shared::UniformDataRecord& CW::Renderer::Shared:: UniformDataRecord::operator=(const UniformDataRecord& second) noexcept {
  if(this == &second) return *this;
  
  value = second.value; 
  size = second.size;
  type = second.type;

  return *this;
};



// move
inline CW::Renderer::Shared::UniformDataRecord::UniformDataRecord(UniformDataRecord&& second) noexcept 
  : value(std::move(second.value)), size(std::move(second.size)), type(std::move(second.type)){
  second.type = &typeid(float);
  second.value = 0.0f;
  second.size = 1;
};



inline CW::Renderer::Shared::UniformDataRecord& CW::Renderer::Shared:: UniformDataRecord::operator=(UniformDataRecord&& second) noexcept {
if(this == &second) return *this;
  
  value = std::move(second.value); 
  size = std::move(second.size);
  type = std::move(second.type);

  second.type = &typeid(float);
  second.value = 0.0f;
  second.size = 1;

  return *this;
};



// =============================
// ======= Constructors ========
// =============================
template<typename T>
inline void CW::Renderer::Shared::UniformDataRecord::set(T value) noexcept {
  this->value = value;
  this->type = &typeid(T);
};



template<typename T>
inline void CW::Renderer::Shared::UniformDataRecord::set(T value, unsigned int size) noexcept {
  this->value = value;
  this->size = size;
  this->type = &typeid(T);
};



template<typename T>
inline T CW::Renderer::Shared::UniformDataRecord::get() const noexcept {
  return std::get<T>(value);
};



inline const std::type_info *CW::Renderer::Shared::UniformDataRecord::getType() const noexcept {
  return type;
};



inline const CW::Renderer::Shared::UniformDataVariants& CW::Renderer::Shared::UniformDataRecord::getValue() const noexcept {
  return value;
};



inline unsigned int CW::Renderer::Shared::UniformDataRecord::getSize() const noexcept{
  return size;
};
