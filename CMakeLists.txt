cmake_minimum_required(VERSION 3.15)


project(Main LANGUAGES CXX C)

add_subdirectory(CWindow)
set(src "Main.cpp")

add_executable(Main ${src} CWindow)