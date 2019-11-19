cmake_minimum_required(VERSION 3.14)
project(mst)

set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)
include_directories("/usr/local/include" "/usr/local/opt/llvm/include")
link_directories("/usr/local/lib" "/usr/local/opt/llvm/lib")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

add_subdirectory(src)
