cmake_minimum_required(VERSION 3.15)

set(src 
  "Renderer.cpp"
  "Renderer.h"
  "Framebuffer.cpp"
  "Mesh/Mesh.cpp"
  "Mesh/Mesh.hpp"
  "Mesh/Mesh.h"
  "Mesh/MeshData.h"
  "Mesh/MeshData.cpp"
  "FreeCamera/FreeCamera3D.cpp"
  "FreeCamera/FreeCamera3D.h"
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
  "Texture/TextureLoader.cpp"
  "Texture/TextureLoader.h"
  ${ROOT_DIR}/vendor/stb/deprecated/stb_image.c
  ${ROOT_DIR}/vendor/glad/src/glad.c
)

set(headers
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}
  ${ROOT_DIR}/vendor/glad/include/  
  ${ROOT_DIR}/vendor/glfw/include/
  ${ROOT_DIR}/vendor/glm/glm/
  ${ROOT_DIR}/vendor/stb/
  ${ROOT_DIR}/vendor/assimp/include/
)

############# [Set Files] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(libs 
  glfw 
  glm
  assimp
  X11
  Xrandr
  Xcursor
  Xi
)

### WIN32
elseif(PLATFORM STREQUAL "WIN32")
set(libs 
  glfw 
  glm
  assimp
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
target_link_libraries(Renderer ${libs})
    
  
