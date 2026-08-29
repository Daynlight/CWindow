# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)



############## [Set Global Properties] #############
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



############# [Set Build] #############
# [OPENGL/VULKAN/DIRECTX]
if(NOT DEFINED RENDERER)
  set(RENDERER "OPENGL")
endif()

# [UNIX/WIN32]
if(NOT DEFINED PLATFORM)
  if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(PLATFORM "WIN32")
  elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(PLATFORM "UNIX")
  else()
    message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
  endif()
endif()

message(STATUS, "Used Platform: ${PLATFORM}")
message(STATUS, "Used Renderer: ${RENDERER}")



############# [Setup project] #############
project(CWindow LANGUAGES CXX C)

### Vendor Libs
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/googletests vendor/googletests EXCLUDE_FROM_ALL)


### Sub Projects
add_subdirectory(Renderer)
add_subdirectory(Gui)


add_library(CWindow STATIC Renderer Gui)
target_link_libraries(CWindow PUBLIC Renderer Gui)