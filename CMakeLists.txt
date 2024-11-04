cmake_minimum_required(VERSION 3.26.4)

project(slip-test LANGUAGES C CXX)
add_library(slip STATIC ./src/slip.cpp)
include_directories(./src)
add_executable(slip-test ./test/test.cpp)
target_link_libraries(slip-test slip)
