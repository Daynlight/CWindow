cmake_minimum_required(VERSION 3.15)

project(Gui LANGUAGES CXX C)

set(src 
  "Gui.cpp"
  ${ROOT_DIR}/vendor/imgui/imgui.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_draw.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_tables.cpp
  ${ROOT_DIR}/vendor/imgui/imgui_widgets.cpp
  ${ROOT_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
  ${ROOT_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(Gui ${src})


target_include_directories(Gui PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_DIR}/Renderer/
    ${ROOT_DIR}/vendor/glad/glad-4.3/include/  
    ${ROOT_DIR}/vendor/glfw/include/
    ${ROOT_DIR}/vendor/glm/glm/
    ${ROOT_DIR}/vendor/imgui/
    ${ROOT_DIR}/vendor/imgui/backends/
  )
  
target_link_libraries(Gui    
    glfw 
    glm
    X11
    Xrandr
    Xcursor
    Xi
  )

