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
set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/x64-linux")

# Tells CMake where to find *-config.cmake packages
list(APPEND CMAKE_PREFIX_PATH "${VCPKG_INSTALLED_DIR}")

# Tells CMake where to find Find*.cmake module packages (like FindStb.cmake)
file(GLOB VCPKG_SHARE_DIRS "${VCPKG_INSTALLED_DIR}/share/*")
list(APPEND CMAKE_MODULE_PATH ${VCPKG_SHARE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GTest CONFIG REQUIRED)


### Sub Projects
add_subdirectory(Renderer)
add_subdirectory(Gui)


add_library(CWindow STATIC Renderer Gui)
target_link_libraries(CWindow PUBLIC Renderer Gui)