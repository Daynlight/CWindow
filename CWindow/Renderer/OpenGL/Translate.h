// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <glad/glad.h>

#include "../Shared/Mesh/MeshDataRecord.h"



namespace CW::Renderer{
inline GLenum TypeToOpenGL(CW::Renderer::Shared::MeshDataType type) noexcept {
  switch (type) {
    case CW::Renderer::Shared::MeshDataType::Float: return GL_FLOAT;
    case CW::Renderer::Shared::MeshDataType::Int:   return GL_INT;
    case CW::Renderer::Shared::MeshDataType::UInt:  return GL_UNSIGNED_INT;
    case CW::Renderer::Shared::MeshDataType::Byte:  return GL_BYTE;
    case CW::Renderer::Shared::MeshDataType::UByte: return GL_UNSIGNED_BYTE;
  };

  return GL_FLOAT;
};

inline GLenum RenderTypeToOpenGL(CW::Renderer::Shared::RenderType type) noexcept {
  switch(type) {
    case CW::Renderer::Shared::RenderType::Triangles:    return GL_TRIANGLES;
    case CW::Renderer::Shared::RenderType::Lines:        return GL_LINES;
    case CW::Renderer::Shared::RenderType::Points:       return GL_POINTS;
    case CW::Renderer::Shared::RenderType::TriangleStrip:return GL_TRIANGLE_STRIP;
    case CW::Renderer::Shared::RenderType::LineStrip:    return GL_LINE_STRIP;
  };

  return GL_TRIANGLES;
};
};
