cmake_minimum_required(VERSION 3.26)
project(image-processor)

set(CMAKE_CXX_STANDARD 14)

include_directories(src, examples, input, cmake-build-debug/output)

add_executable(image-processor
        src/Header.h
        src/TgaFile.cpp
        src/TgaFile.h
        src/main.cpp)
