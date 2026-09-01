# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



set(src 
  "Renderer.cpp"
  "RendererEvents.cpp"
  "Renderer.h"
  "Translate.h"
  "Framebuffer/Framebuffer.cpp"
  "Framebuffer/Framebuffer.h"
  "FreeCamera/FreeCamera3D.cpp"
  "FreeCamera/FreeCamera3D.h"
  "Mesh/Mesh.cpp"
  "Mesh/Mesh.h"
  "Shader/Shader/Shader.cpp"
  "Shader/Shader/Shader.h"
  "Shader/Shader/ShaderData.cpp"
  "Shader/Shader/ShaderData.h"
  "Shader/Compute/Shader.cpp"
  "Shader/Compute/Shader.hpp"
  "Shader/Compute/Shader.h"
  "Shader/SSBO/Shader.cpp"
  "Shader/SSBO/Shader.hpp"
  "Shader/SSBO/Shader.h"
  "Uniform/Uniform.cpp"
  "Uniform/Uniform.h"
  "Texture/Texture.cpp"
  "Texture/Texture.h"
  "../Interface/Mesh/iMesh.h"
  "../Shared/Texture/TextureLoader.cpp"
  "../Shared/Texture/TextureLoader.h"
  "../Shared/Mesh/MeshDataRecord.h"
  "../Shared/Mesh/MeshDataRecord.cpp"
  "../Shared/Mesh/MeshData.cpp"
  "../Shared/Mesh/MeshData.h"
  "../Shared/Mesh/MeshData.hpp"
  "../Shared/Translate.h"
)


set(headers
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}
  ../Shared/
  ../Interface/
  ${Stb_INCLUDE_DIR}
)



############# [Set Files] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(libs 
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  X11
  Xrandr
  Xcursor
  Xi
)

### WIN32
elseif(PLATFORM STREQUAL "WIN32")
set(libs 
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  winmm
  imm32
  version
  setupapi
)

else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()



############# [Create Renderer] #############
project(Renderer LANGUAGES CXX)

add_library(Renderer STATIC ${src})
target_include_directories(Renderer PUBLIC ${headers})
target_link_libraries(Renderer PRIVATE ${libs})
