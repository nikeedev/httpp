cmake_minimum_required(VERSION 3.8)
project(test)

add_executable(test main.cpp)

include_directories(../src)
target_link_libraries(test PRIVATE httpp)