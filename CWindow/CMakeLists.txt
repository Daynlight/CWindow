# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

project(CWindow LANGUAGES CXX C)


############# [Set Libs] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(libs_gui
  imgui::imgui
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  X11
  Xrandr
  Xcursor
  Xi
)
set(libs_renderer 
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  X11
  Xrandr
  Xcursor
  Xi
)
### WIN32
elseif(PLATFORM STREQUAL "WIN32")
set(libs_gui
  imgui::imgui
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  winmm
  imm32
  version
  setupapi
)
set(libs_renderer
  glfw 
  glm::glm
  assimp::assimp
  glad::glad
  winmm
  imm32
  version
  setupapi
)
else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()


### Add Subdirectories
add_subdirectory(Renderer)
add_subdirectory(Gui)


add_library(CWindow STATIC 
  $<TARGET_OBJECTS:Renderer>
  $<TARGET_OBJECTS:Gui>
)

add_library(CWindow::CWindow ALIAS CWindow)
target_link_libraries(CWindow PRIVATE ${libs_renderer} ${libs_gui})

target_include_directories(
  CWindow
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)
