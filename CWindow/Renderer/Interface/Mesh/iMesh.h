// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include "../../Shared/Mesh/MeshData.h"
#include "../../Shared/Translate.h"



namespace CW::Renderer::Interface{
class iMesh{
public:
  virtual void setMeshData(const CW::Renderer::Shared::MeshData& mesh_data) noexcept = 0;
  virtual const CW::Renderer::Shared::MeshData& getMeshData() const noexcept = 0;
  virtual void compile() noexcept = 0;
  virtual void destroy() noexcept = 0;
  virtual void render(CW::Renderer::Shared::RenderType type = CW::Renderer::Shared::RenderType::Triangles) noexcept = 0;
};
};