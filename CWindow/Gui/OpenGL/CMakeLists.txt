cmake_minimum_required(VERSION 3.15)

set(src 
  "Gui.cpp"
  "../iGui.h"
  "../GuiWindow/GuiWindow.cpp"
  ${ROOT_DIR}/vendor/imgui/imgui.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_draw.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_tables.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_widgets.cpp
  ${ROOT_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
  ${ROOT_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
)
set(headers
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}
  ${ROOT_DIR}/Renderer/
  ${ROOT_DIR}/vendor/glad/glad-4.3/include/  
  ${ROOT_DIR}/vendor/glfw/include/
  ${ROOT_DIR}/vendor/glm/glm/
  ${ROOT_DIR}/vendor/imgui/
  ${ROOT_DIR}/vendor/imgui/backends/
)

############# [Set Files] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(libs 
  glfw 
  glm
  X11
  Xrandr
  Xcursor
  Xi
)

### WIN32
elseif(PLATFORM STREQUAL "WIN32")
set(libs 
  glfw 
  glm
  winmm
  imm32
  version
  setupapi
)

### MACOS
elseif(PLATFORM STREQUAL "MACOS")
set(libs 
  glfw 
  glm
  Cocoa
  OpenGL
  CoreFoundation
)

else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()




############# [Create Renderer] #############
project(Gui LANGUAGES CXX C)
add_library(Gui STATIC ${src})
target_include_directories(Gui PUBLIC ${headers})
target_link_libraries(Gui ${libs})

