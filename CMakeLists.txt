cmake_minimum_required(VERSION 3.15)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output)

project(Main LANGUAGES CXX C)

add_subdirectory(CWindow)
set(src "Main.cpp")
set(headers "Mandelbrot.h")

add_executable(Main ${src})
target_link_libraries(Main CWindow)