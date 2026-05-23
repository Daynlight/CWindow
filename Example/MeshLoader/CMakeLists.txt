cmake_minimum_required(VERSION 3.15)

project(MeshLoaderExample LANGUAGES CXX C)

set(src "Main.cpp")
set(headers 
  "Shaders.h"
)

add_executable(MeshLoaderExample ${src})
target_link_libraries(MeshLoaderExample CWindow)