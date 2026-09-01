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


############# [Create Renderer] #############
project(Gui LANGUAGES CXX C)
add_library(Gui STATIC ${src})
target_include_directories(Gui PUBLIC ${headers})
target_link_libraries(Gui ${libs_gui})
