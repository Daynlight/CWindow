// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#include "MeshData.h"



// =============================
// ======= Constructors ========
// =============================
// core
CW::Renderer::Shared::MeshData::MeshData() noexcept {};



CW::Renderer::Shared::MeshData::~MeshData() noexcept {};



// copy
CW::Renderer::Shared::MeshData::MeshData(const MeshData& second) noexcept
  : dataRegister(second.dataRegister), indices(second.indices),
    bufferData(second.bufferData), culling_box(second.culling_box),
    culling_box_exists(second.culling_box_exists), 
    line_size(second.line_size), buffer_is_ready(second.buffer_is_ready),
    keys(second.keys), keys_are_ready(second.keys_are_ready),
    version(second.version) {};



CW::Renderer::Shared::MeshData& CW::Renderer::Shared::MeshData::operator=(const MeshData& second) noexcept {
  if(this == &second) return *this;

  dataRegister = second.dataRegister;
  indices = second.indices;
  bufferData = second.bufferData;
  culling_box = second.culling_box;
  culling_box_exists = second.culling_box_exists;
  line_size = second.line_size;
  buffer_is_ready = second.buffer_is_ready;
  keys = second.keys; 
  keys_are_ready = second.keys_are_ready;
  version = second.version;

  return* this;
};



// move
CW::Renderer::Shared::MeshData::MeshData(MeshData&& second) noexcept
  : dataRegister(std::move(second.dataRegister)), indices(std::move(second.indices)),
    bufferData(std::move(second.bufferData)), culling_box(std::move(second.culling_box)),
    culling_box_exists(std::move(second.culling_box_exists)), 
    line_size(std::move(second.line_size)), buffer_is_ready(std::move(second.buffer_is_ready)),
    keys(std::move(second.keys)), keys_are_ready(std::move(second.keys_are_ready)),
    version(std::move(second.version)) {
  second.version = -1;
};



CW::Renderer::Shared::MeshData& CW::Renderer::Shared::MeshData::operator=(MeshData&& second) noexcept {
  if(this == &second) return *this;

  dataRegister = std::move(second.dataRegister);
  indices = std::move(second.indices);
  bufferData = std::move(second.bufferData);
  culling_box = std::move(second.culling_box);
  culling_box_exists = std::move(second.culling_box_exists);
  line_size = std::move(second.line_size);
  buffer_is_ready = std::move(second.buffer_is_ready);
  keys = std::move(second.keys);
  keys_are_ready = std::move(second.keys_are_ready);
  version = std::move(second.version);

  second.version = -1;

  return* this;
};



// =============================
// ======= Data Control ========
// =============================
void CW::Renderer::Shared::MeshData::addVertices(const std::vector<float>& vertices, const unsigned int dimension, const unsigned int layout) noexcept {
  if(dimension == 0) return;
  culling_box_exists = false;

  setData<float>(vertices, dimension, layout, CW::Renderer::Shared::MeshDataType::Float);
  generateCullingBox(vertices, dimension);
  
  buffer_is_ready = false;
  keys_are_ready = false;
  version++;
};



void CW::Renderer::Shared::MeshData::addIndices(const std::vector<unsigned int>& indices) noexcept {
  this->indices = indices;
  version++;
};



const std::vector<unsigned int> &CW::Renderer::Shared::MeshData::getIndices() const noexcept{
  return indices;
};



void CW::Renderer::Shared::MeshData::removeData(const unsigned int layout) noexcept {
  this->dataRegister.erase(layout);
  buffer_is_ready = false;
  keys_are_ready = false;
  version++;
};



void CW::Renderer::Shared::MeshData::clearData() noexcept {
  this->dataRegister.clear();
  buffer_is_ready = false;
  keys_are_ready = false;
  version++;
};



bool CW::Renderer::Shared::MeshData::getCullingBoxExists() const noexcept {
  return culling_box_exists;
};



const std::array<std::vector<float>, 2>& CW::Renderer::Shared::MeshData::getCullingBox() const noexcept {
  return culling_box;
};



const std::unordered_map<unsigned int, CW::Renderer::Shared::MeshDataRecord> &CW::Renderer::Shared::MeshData::getDataRegister() const noexcept{
  return dataRegister;
};



const std::vector<unsigned int>& CW::Renderer::Shared::MeshData::getDataRegisterLayouts() noexcept {
  if(keys_are_ready) return keys;

  keys.clear();

  keys.reserve(dataRegister.size());
  for (const std::pair<const unsigned int, MeshDataRecord> &pair : dataRegister)
    keys.emplace_back(pair.first);

  std::sort(keys.begin(), keys.end());

  keys_are_ready = true;
  version++;
  return keys;
};



void CW::Renderer::Shared::MeshData::generateBuffer() {
  if(buffer_is_ready) return;

  buffer_is_ready = false;
  line_size = 0;
  unsigned int total_size = 0;

  if(indices.size() == 0) throw std::runtime_error("CW::Renderer::Mesh::compile: indices.size() == 0, no data to create MeshData");
  
  for(const std::pair<const unsigned int, MeshDataRecord>& el : dataRegister){
    line_size += el.second.getDimension() * el.second.getSizeOfElement();
    total_size += el.second.getSize();
  };

  if(line_size == 0) throw std::runtime_error("CW::Renderer::Mesh::compile: line_size == 0 no data to add");
  const unsigned int total_points = total_size / line_size;
      
  const std::vector<unsigned int>& keys = getDataRegisterLayouts();
  
  bufferData.clear();
  bufferData.resize(total_size);

  unsigned int dstOffset = 0;

  for (unsigned int i = 0; i < total_points; ++i) {
    for (unsigned int k = 0; k < keys.size(); ++k) {
      const CW::Renderer::Shared::MeshDataRecord& md = dataRegister.at(keys[k]);
      const unsigned int elementSize = md.getDimension() * md.getSizeOfElement();
      const unsigned int srcOffset = i * elementSize;

      std::memcpy(bufferData.data() + dstOffset, md.getRawData() + srcOffset, elementSize);

      dstOffset += elementSize;
    };
  };
  
  version++;
  buffer_is_ready = true;
};



size_t CW::Renderer::Shared::MeshData::getLineSize() const noexcept {
  return line_size;
};



const std::vector<char> &CW::Renderer::Shared::MeshData::getDataBuffer() const noexcept {
  return bufferData;
};



unsigned int CW::Renderer::Shared::MeshData::getVersion() const noexcept{
  return version;
};



// =============================
// ========== Helpers ==========
// =============================
void CW::Renderer::Shared::MeshData::generateCullingBox(const std::vector<float>& data, const unsigned int dimension) noexcept {
  culling_box[0] = {0};
  culling_box[1] = {0};
  culling_box_exists = false;
  
  if(dimension == 0) return;
  
  std::vector<float> vertex_max;
  vertex_max.reserve(dimension);
  std::vector<float> vertex_min;
  vertex_min.reserve(dimension);
  
  for(unsigned int i = 0; i < dimension; i++){
    vertex_max.emplace_back(data[i]);
    vertex_min.emplace_back(data[i]);
  };

  for(unsigned int i = 1; i < data.size() / dimension; i++){
    for(unsigned int j = 0; j < dimension; j++){
      if(vertex_max[j] < data[dimension * i + j])
        vertex_max[j] = data[dimension * i + j];
      if(vertex_min[j] > data[dimension * i + j])
        vertex_min[j] = data[dimension * i + j];
    };
  };

  culling_box[0] = vertex_min;
  culling_box[1] = vertex_max;
  culling_box_exists = true;
};