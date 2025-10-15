cmake_minimum_required(VERSION 3.15)

############## [Set Global Properties] #############
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



############# [Set Variables] #############
# [OPENGL/VULKAN/DIRECTX/METAL]
set(RENDERER "OPENGL") 
# [UNIX/WIN32/MACOS]
set(PLATFORM "WIN32")

# Validate RENDERER
if(NOT RENDERER STREQUAL "OPENGL" 
  AND NOT RENDERER STREQUAL "VULKAN" 
  AND NOT RENDERER STREQUAL "DIRECTX"
  AND NOT RENDERER STREQUAL "METAL")
    message(FATAL_ERROR "Invalid RENDERER specified: ${RENDERER}.")
endif()

# Validate PLATFORMS
if(NOT PLATFORM STREQUAL "WIN32" 
  AND NOT PLATFORM STREQUAL "UNIX" 
  AND NOT PLATFORM STREQUAL "MACOS")
    message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()

message(STATUS "Configured Renderer: ${RENDERER}")
message(STATUS "Configured Platform: ${PLATFORM}")



############# [Setup main project] #############

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)
add_subdirectory(Renderer)
add_subdirectory(Gui)

set(src "Main.cpp")
set(headers "Macro.h")

project(Main LANGUAGES CXX C)
add_executable(Main ${src})
target_link_libraries(Main Renderer Gui)
link_directories(${headers})
