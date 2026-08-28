# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

set(tests_src
  Unit/Renderer.cpp
)

add_executable(unit_tests ${tests_src})


target_link_libraries(unit_tests PRIVATE 
  CWindow  
  gtest_main
  gmock_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)