cmake_minimum_required(VERSION 3.15)

project(Texture LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(Texture ${src})
target_link_libraries(Texture CWindow)