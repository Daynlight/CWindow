cmake_minimum_required(VERSION 3.15)

project(OpenGLRenderer LANGUAGES CXX)

set(src 
  "OpenGL_Renderer.cpp"
  ${ROOT_DIR}/vendor/glad/glad-4.3/src/glad.c
  )
  
set(headers
  "iRenderer.h"
)
add_library(OpenGLRenderer ${src})

target_include_directories(OpenGLRenderer PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_DIR}/
    ${ROOT_DIR}/vendor/glad/glad-4.3/include/  
    ${ROOT_DIR}/vendor/glfw/include/
    ${ROOT_DIR}/vendor/glm/glm/
    ${headers}
  )
  
target_link_libraries(OpenGLRenderer 
    glfw 
    glm
    X11
    Xrandr
    Xcursor
    Xi
  )

