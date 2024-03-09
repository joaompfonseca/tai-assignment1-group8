cmake_minimum_required(VERSION 3.0)
project(tai_assignment1_group8/src)

set(CMAKE_CXX_STANDARD 17)

add_executable(test src/cpm.cpp)
add_executable(cpm src/cpm/main.cpp)
add_executable(mutate src/mutate/main.cpp)
