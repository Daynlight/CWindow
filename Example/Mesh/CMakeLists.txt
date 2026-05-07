cmake_minimum_required(VERSION 3.15)

project(MeshExample LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(MeshExample ${src})
target_link_libraries(MeshExample CWindow)