# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(CWindow LANGUAGES CXX C)


add_subdirectory(Renderer)
add_subdirectory(Gui)


add_library(CWindow STATIC 
  $<TARGET_OBJECTS:Renderer>
  $<TARGET_OBJECTS:Gui>
)

add_library(Crypt::Crypt ALIAS Crypt)


target_link_libraries(CWindow
  PUBLIC
  Renderer
  Gui
)

target_include_directories(
  CWindow
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)
