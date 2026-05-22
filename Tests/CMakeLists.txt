cmake_minimum_required(VERSION 3.15)

enable_testing()

project(Tests LANGUAGES CXX C)

set(src
  "main.cpp"
)

set(headers
  "Tests.h"
)

add_executable(Tests ${src})
target_include_directories(Tests PUBLIC ${headers})
target_link_libraries(Tests CWindow)

add_test(NAME Tests COMMAND Tests)