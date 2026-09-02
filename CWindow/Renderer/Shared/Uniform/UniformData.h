// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <unordered_map>
#include <string>

#include "UniformDataRecord.h"
#include "../Translate.h"



namespace CW::Renderer::Shared{
class UniformData{
// ===============================================
// ==================== Data =====================
// ===============================================
private:
  std::unordered_map<std::string, UniformDataRecord> data;



// ===============================================
// ================== Functions ==================
// ===============================================
// =============================
// ======= Constructors ========
// =============================
public:
  // core
  UniformData() noexcept;
  ~UniformData() noexcept;
  // copy
  UniformData(const UniformData& second) noexcept;
  UniformData& operator=(const UniformData& second) noexcept;
  // move
  UniformData(UniformData&& second) noexcept;
  UniformData& operator=(UniformData&& second) noexcept;

// =============================
// ======= Data Control ========
// =============================
  UniformDataRecord& at(const std::string& name) noexcept;
  UniformDataRecord& operator[](const std::string& name) noexcept;
  void remove(const std::string& name) noexcept;
  void clear() noexcept;
  const std::unordered_map<std::string, UniformDataRecord>& getData();
};
};
