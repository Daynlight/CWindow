cmake_minimum_required(VERSION 3.15)


project(Tests LANGUAGES CXX)

set(src "Main.cpp")

add_executable(Tests ${src})
target_link_libraries(Tests CWindow)