cmake_minimum_required(VERSION 3.0)
project(tai_assignment1_group8/src)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -O3")

add_executable(cpm src/cpm.cpp
        src/reader/FileReader.cpp
        src/reader/FileReader.h
        src/copymodel/CopyModel.cpp
        src/copymodel/CopyModel.h
        src/copymodel/CopyModelRunner.cpp
        src/copymodel/CopyModelRunner.h)

add_executable(mutate src/mutate.cpp)
