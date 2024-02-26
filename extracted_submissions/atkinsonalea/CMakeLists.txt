cmake_minimum_required(VERSION 3.26)
project(image-processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(image-processor
        src/imageprocessor.cpp
        src/Image.h
        src/Image.cpp)
