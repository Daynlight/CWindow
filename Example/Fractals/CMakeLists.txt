cmake_minimum_required(VERSION 3.15)

project(Fractals LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(Fractals ${src})
target_link_libraries(Fractals CWindow)