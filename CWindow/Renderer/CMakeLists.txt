cmake_minimum_required(VERSION 3.15)




if(RENDERER STREQUAL "OPENGL")
  add_subdirectory(OpenGL)
else()
  message(FATAL_ERROR "Invalid RENDERER specified: ${RENDERER}.")
endif()
