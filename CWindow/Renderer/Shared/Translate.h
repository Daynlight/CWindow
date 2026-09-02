// CWindow
// Copyright 2026 Daynlight
// Licensed under the GNU General.
// See LICENSE file for details.



#pragma once
#include <variant>
#include <typeinfo>
#include <glm/glm.hpp>



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


// using UniformDataVariants = std::variant<int, int[2], int[3], int[4],
//                                   float, float[2], float[3], float[4], 
//                                   double, double[2], double[3], double[4], 
//                                   float[2][2], float[3][3], float[4][4]>;

using UniformDataVariants = std::variant<int, glm::ivec2, glm::ivec3, glm::ivec4,
                                  float, glm::vec2, glm::vec3, glm::vec4, 
                                  double, glm::dvec2, glm::dvec3, glm::dvec4, 
                                  glm::mat2, glm::mat3, glm::mat4>;

};