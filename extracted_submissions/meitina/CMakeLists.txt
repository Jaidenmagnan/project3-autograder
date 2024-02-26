cmake_minimum_required(VERSION 3.22)
project(image-processor)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(image-processor
        src/Header.cpp
        src/Header.h
        src/image_processor.cpp
        src/ImageData.h
        src/ImageData.cpp
)
