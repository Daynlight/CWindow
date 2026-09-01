// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once



namespace CW::Renderer::Shared{
enum class MeshDataType {
  Float,
  Int,
  UInt,
  Byte,
  UByte
};



enum class RenderType {
  Triangles,
  Lines,
  Points,
  TriangleStrip,
  LineStrip
};
};