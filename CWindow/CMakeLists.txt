cmake_minimum_required(VERSION 3.15)

############## [Set Global Properties] #############
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



############# [Set Variables] #############
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

message("Used Platform: ${PLATFORM}")
message("Used Renderer: ${RENDERER}")



############# [Setup project] #############
project(CWindow LANGUAGES CXX C)

### Vendor Libs
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)


### Subfolders
add_subdirectory(Renderer)
add_subdirectory(Gui)

set(src "CWindow.cpp")

add_library(CWindow STATIC ${src})
target_link_libraries(CWindow  Renderer Gui)