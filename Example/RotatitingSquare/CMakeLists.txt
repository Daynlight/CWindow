cmake_minimum_required(VERSION 3.15)

project(RotatingSquare LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(RotatingSquare ${src})
target_link_libraries(RotatingSquare CWindow)