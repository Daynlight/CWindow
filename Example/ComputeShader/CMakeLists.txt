cmake_minimum_required(VERSION 3.15)

project(ComputeShader LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(ComputeShader ${src})
target_link_libraries(ComputeShader CWindow)