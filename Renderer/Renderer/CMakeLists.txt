cmake_minimum_required(VERSION 3.15)

############# [Set Files] #############
### UNIX
if(PLATFORM STREQUAL "UNIX")
set(src 
  "OpenGL_Renderer.cpp"
  ${ROOT_DIR}/vendor/glad/glad-4.3/src/glad.c
)
set(headers
  "iRenderer.h"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}/
  ${ROOT_DIR}/vendor/glad/glad-4.3/include/  
  ${ROOT_DIR}/vendor/glfw/include/
  ${ROOT_DIR}/vendor/glm/glm/
)
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
set(src 
  "OpenGL_Renderer.cpp"
  ${ROOT_DIR}/vendor/glad/glad-4.3/src/glad.c
)
set(headers
  "iRenderer.h"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${ROOT_DIR}/
  ${ROOT_DIR}/vendor/glad/glad-4.3/include/  
  ${ROOT_DIR}/vendor/glfw/include/
  ${ROOT_DIR}/vendor/glm/glm/
)
set(libs 
  glfw 
  glm
  winmm
  imm32
  version
  setupapi
)


else()
  message(FATAL_ERROR "Invalid PLATFORM specified: ${PLATFORM}.")
endif()



############# [Create Renderer] #############
project(Renderer LANGUAGES CXX)
add_library(Renderer ${src})
target_include_directories(Renderer PUBLIC ${headers})
target_link_libraries(Renderer ${libs})
    
  
