// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include "../Translate.h"



namespace CW::Renderer::Shared{
class UniformDataRecord{
// ===============================================
// ==================== Data =====================
// ===============================================
private:
  UniformDataVariants value = 0.0f;
  unsigned int size = 1;
  const std::type_info* type = &typeid(float);



// ===============================================
// ================== Functions ==================
// ===============================================
// =============================
// ======= Constructors ========
// =============================
public:  
  // core
  UniformDataRecord() noexcept;
  ~UniformDataRecord() noexcept;
  // copy
  UniformDataRecord(const UniformDataRecord& second) noexcept;
  UniformDataRecord& operator=(const UniformDataRecord& second) noexcept;
  // move
  UniformDataRecord(UniformDataRecord&& second) noexcept;
  UniformDataRecord& operator=(UniformDataRecord&& second) noexcept;

// =============================
// ======= Constructors ========
// =============================
public:
  template<typename T>
  void set(T value) noexcept;

  template<typename T>
  void set(T value, unsigned int size) noexcept;

  template<typename T>
  T get() const noexcept;

  const std::type_info* getType() const noexcept;
  const UniformDataVariants& getValue() const noexcept;
  unsigned int getSize() const noexcept;
};
};



#include "UniformDataRecord.hpp"
