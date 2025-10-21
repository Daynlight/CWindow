cmake_minimum_required(VERSION 3.15)

project(ShaderAndMesh LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(ShaderAndMesh ${src})
target_link_libraries(ShaderAndMesh CWindow)