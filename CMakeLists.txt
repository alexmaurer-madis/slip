cmake_minimum_required(VERSION 3.26.4)
set(CMAKE_BUILD_TYPE Debug)

# set( CMAKE_CXX_COMPILER g++ )
# set( CMAKE_CC_COMPILER gcc )
# set( CMAKE_cpp_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )
# set( CMAKE_MAKE_PROGRAM mingw32-make)

project(slip-test LANGUAGES C CXX)
add_library(slip STATIC ./src/slip.cpp ./src/slip.hpp)
add_executable(test-slip ./src/test.cpp)
target_link_libraries(test-slip slip)

# include(CTest)
# enable_testing()