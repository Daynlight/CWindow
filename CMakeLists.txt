cmake_minimum_required(VERSION 3.15)

############## [Set Global Properties] #############
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root directory of the project")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



############# [Set Variables] #############
# [OPENGL/VULKAN/DIRECTX/METAL]
# set(RENDERER "OPENGL") 
# [UNIX/WIN32/MACOS]
# set(PLATFORM "WIN32")

message("Used Renderer: ${RENDERER}")
message("Used Platform: ${PLATFORM}")

############# [Setup project] #############
project(Main LANGUAGES CXX C)

### Vendor Libs
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glm)

### Subfolders
add_subdirectory(Renderer)
add_subdirectory(Gui)

### Files
set(src "Main.cpp")
set(headers "Macro.h")

### Setup Main Project
if(PLATFORM STREQUAL "UNIX")
  add_executable(Main ${src})
elseif(PLATFORM STREQUAL "WIN32")
  add_executable(Main WIN32 ${src})
else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()

### Link libs and directories
target_link_libraries(Main Renderer Gui)
link_directories(${headers})
