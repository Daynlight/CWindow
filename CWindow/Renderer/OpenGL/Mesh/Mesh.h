// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include "glad/glad.h"

#include <unordered_map>
#include <vector>

#include "../../Interface/Mesh/iMesh.h"

#include "../Translate.h"

#include "../../Shared/Translate.h"
#include "../../Shared/Mesh/MeshDataRecord.h"
#include "../../Shared/Mesh/MeshData.h"



namespace CW::Renderer{
class Mesh : public CW::Renderer::Interface::iMesh{
// ===============================================
// ==================== Data =====================
// ===============================================
private:
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;
  bool is_compiled = false;
  CW::Renderer::Shared::MeshData mesh_data;
  unsigned int mesh_data_version = -1;



// ===============================================
// ================== Functions ==================
// ===============================================
// =============================
// ======= Constructors ========
// =============================
public:
  // core
  Mesh() noexcept;
  Mesh(const CW::Renderer::Shared::MeshData& mesh_data) noexcept;
  ~Mesh() noexcept;
  // copy
  Mesh(const Mesh& second) noexcept;
  Mesh& operator=(const Mesh& second) noexcept;
  // move
  Mesh(Mesh&& second) noexcept;
  Mesh& operator=(Mesh&& second) noexcept;

// =============================
// ======= Data Control ========
// =============================
public:
  void setMeshData(const CW::Renderer::Shared::MeshData& mesh_data) noexcept override;
  const CW::Renderer::Shared::MeshData& getMeshData() const noexcept override;
  void compile() noexcept override;
  void destroy() noexcept override;
  void render(CW::Renderer::Shared::RenderType type = CW::Renderer::Shared::RenderType::Triangles) noexcept override;

// =============================
// ========== Helpers ==========
// =============================
private:
  void genBuffers(const std::vector<char>& bufferData) noexcept;
  void closeBuffers() const noexcept;
  void setDataPositions(const std::vector<unsigned int>& keys, const unsigned int line_size) const noexcept;
};
};
