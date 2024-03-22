project(test)

add_executable(test main.cpp)

include_directories(../src)
target_link_libraries(test PRIVATE httpp)