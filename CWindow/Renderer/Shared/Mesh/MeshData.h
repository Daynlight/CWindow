// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <vector>
#include <cstring>

#include "MeshDataRecord.h"
#include "../Translate.h"



namespace CW::Renderer::Shared{
class MeshData{
// ===============================================
// ==================== Data =====================
// ===============================================
private:
  std::unordered_map<unsigned int, CW::Renderer::Shared::MeshDataRecord> dataRegister;
  std::vector<unsigned int> indices;
  std::vector<char> bufferData;
  unsigned int line_size = 0;
  
  std::array<std::vector<float>, 2> culling_box;
  std::vector<unsigned int> keys;
  bool culling_box_exists = false;
  bool buffer_is_ready = false;
  bool keys_are_ready = false;
  unsigned int version = -1;



// ===============================================
// ================== Functions ==================
// ===============================================
// =============================
// ======= Constructors ========
// =============================
public:
  // core
  MeshData() noexcept;
  ~MeshData() noexcept;
  // copy
  MeshData(const MeshData& second) noexcept;
  MeshData& operator=(const MeshData& second) noexcept;
  // move
  MeshData(MeshData&& second) noexcept;
  MeshData& operator=(MeshData&& second) noexcept;

// =============================
// ======= Data Control ========
// =============================
public:
  void addVertices(const std::vector<float>& vertices, const unsigned int dimension = 4, const unsigned int layout = 0) noexcept;
  void addIndices(const std::vector<unsigned int>& indices) noexcept;
  const std::vector<unsigned int>& getIndices() const noexcept;

  template<typename T>
  void setData(const std::vector<T>& data, const unsigned int dimension, const unsigned int layout, CW::Renderer::Shared::MeshDataType type = CW::Renderer::Shared::MeshDataType::Float) noexcept;
  
  void removeData(const unsigned int layout) noexcept;
  void clearData() noexcept;
  
  bool getCullingBoxExists() const noexcept;
  const std::array<std::vector<float>, 2>& getCullingBox() const noexcept;
  
  const std::unordered_map<unsigned int, CW::Renderer::Shared::MeshDataRecord>& getDataRegister() const noexcept;
  const std::vector<unsigned int>& getDataRegisterLayouts() noexcept;

  void generateBuffer();
  size_t getLineSize() const noexcept;
  const std::vector<char>& getDataBuffer() const noexcept;
  unsigned int getVersion() const noexcept;
  
  // =============================
  // ========== Helpers ==========
  // =============================
private:
  void generateCullingBox(const std::vector<float>& data, const unsigned int dimension) noexcept;
};
};



#include "MeshData.hpp"
