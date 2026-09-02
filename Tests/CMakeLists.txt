# CWindow
# Copyright 2026 Daynlight
# Licensed under the GNU General.
# See LICENSE file for details.



cmake_minimum_required(VERSION 3.15)

find_package(GTest REQUIRED CONFIG)

set(tests_src
  Unit/Renderer/Shared/Mesh/MeshDataRecordTests.cpp
  Unit/Renderer/Shared/Mesh/MeshDataTests.cpp
  Unit/Renderer/Shared/Uniform/UniformDataRecord.cpp
  Unit/Renderer/Shared/Uniform/UniformData.cpp
  Unit/Renderer/OpenGL/Mesh/MeshTests.cpp
)

add_executable(unit_tests ${tests_src})


target_link_libraries(unit_tests PRIVATE 
  CWindow::CWindow
  GTest::gtest
  GTest::gtest_main
)


include(GoogleTest)
gtest_discover_tests(unit_tests)