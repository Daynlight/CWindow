cmake_minimum_required(VERSION 3.15)

set(src 
  "Gui.cpp"
  "../Interface/iGui.h"
  "../Shared/GuiWindow/GuiWindow.cpp"
)
set(headers
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}
  ${ROOT_DIR}/Renderer/
  ../Shared/
  ../Interface/
)

############# [Set Files] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(libs
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

### WIN32
elseif(PLATFORM STREQUAL "WIN32")
set(libs 
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

else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()




############# [Create Renderer] #############
project(Gui LANGUAGES CXX C)
add_library(Gui STATIC ${src})
target_include_directories(Gui PUBLIC ${headers})
target_link_libraries(Gui ${libs})

