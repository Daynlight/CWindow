cmake_minimum_required(VERSION 3.15)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CXX_STANDARD 26)
set(CXX_STANDARD_REQUIRED ON)




# [OPENGL/VULKAN/DIRECTX]
set(RENDERER "OPENGL") 
# [${CMAKE_GENERATOR_PLATFORM}(auto)/UNIX/WIN32]
set(PLATFORM ${CMAKE_GENERATOR_PLATFORM})




project(CWindow LANGUAGES CXX C)

set(src 
  "Main.cpp"
)

set(headers
  "Macro.h"
)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(Renderer)
add_subdirectory(Gui)

add_executable(CWindow ${src})

target_link_libraries(CWindow OpenGLRenderer Gui)
link_directories(${headers})
