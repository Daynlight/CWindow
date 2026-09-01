// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <vector>

#include "../Translate.h"



namespace CW::Renderer::Shared{
class MeshDataRecord{
// ===============================================
// ==================== Data =====================
// ===============================================
private:
  std::vector<char> data;
  unsigned int dimension = 0;
  unsigned int size_of_element = 0;
  unsigned int size = 0;
  MeshDataType type = MeshDataType::Float;



// ===============================================
// ================== Functions ==================
// ===============================================
// =============================
// ======= Constructors ========
// =============================
public:
  // core
  MeshDataRecord() noexcept;
  MeshDataRecord(std::vector<char>& data, const unsigned int size, const unsigned int size_of_element, const unsigned int dimension, MeshDataType type) noexcept;
  ~MeshDataRecord() noexcept;
  // copy
  MeshDataRecord(const MeshDataRecord& second) noexcept;
  MeshDataRecord& operator=(const MeshDataRecord& second) noexcept;
  // move
  MeshDataRecord(MeshDataRecord&& other) noexcept;
  MeshDataRecord& operator=(MeshDataRecord&& other) noexcept;

// =============================
// ======= Data Control ========
// =============================
  unsigned int getSize() const noexcept;
  unsigned int getSizeOfElement() const noexcept;
  unsigned int getDimension() const noexcept;
  const char* getRawData() const noexcept;
  MeshDataType getType() const noexcept;
};
};
