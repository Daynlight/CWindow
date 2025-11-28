cmake_minimum_required(VERSION 3.15)

project(Filters LANGUAGES CXX C)

set(src "Main.cpp")
set(headers "Shaders.h")

add_executable(Filters ${src})
target_link_libraries(Filters CWindow)