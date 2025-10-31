cmake_minimum_required(VERSION 3.15)

project(Data LANGUAGES CXX C)

set(src "Main.cpp")

add_executable(Data ${src})
target_link_libraries(Data CWindow)