cmake_minimum_required(VERSION 3.15)

project(Example LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Mandelbrot.h")

add_executable(Example ${src})
target_link_libraries(Example CWindow)