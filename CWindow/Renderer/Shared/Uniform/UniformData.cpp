// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "UniformData.h"



// =============================
// ======= Constructors ========
// =============================
// core
CW::Renderer::Shared::UniformData::UniformData() noexcept {};



CW::Renderer::Shared::UniformData::~UniformData() noexcept {};



// copy
CW::Renderer::Shared::UniformData::UniformData(const UniformData& second) noexcept 
  : data(second.data) {};



CW::Renderer::Shared::UniformData& CW::Renderer::Shared::UniformData::operator=(const UniformData& second) noexcept {
  if(this == &second) return *this;
  data = second.data;
  return *this;
};



// move
CW::Renderer::Shared::UniformData::UniformData(UniformData&& second) noexcept 
  : data(std::move(second.data)) {};



CW::Renderer::Shared::UniformData& CW::Renderer::Shared::UniformData::operator=(UniformData&& second) noexcept {
if(this == &second) return *this;
  data = std::move(second.data);
  return *this;
};



// =============================
// ======= Data Control ========
// =============================
CW::Renderer::Shared::UniformDataRecord& CW::Renderer::Shared::UniformData::at(const std::string& name) noexcept {
  if(!data.contains(name)) data[name] = CW::Renderer::Shared::UniformDataRecord();
  return data[name];
};



CW::Renderer::Shared::UniformDataRecord& CW::Renderer::Shared::UniformData::operator[](const std::string& name) noexcept {
  return this->at(name);
};



void CW::Renderer::Shared::UniformData::remove(const std::string& name) noexcept {
  if (data.contains(name)) data.erase(name);
};



void CW::Renderer::Shared::UniformData::clear() noexcept {
  data.clear();
};



const std::unordered_map<std::string, CW::Renderer::Shared::UniformDataRecord> &CW::Renderer::Shared::UniformData::getData(){
  return data;
};
