cmake_minimum_required(VERSION 3.26)
project(image-processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(image-processor
        src/Header.h
        src/main.cpp
        src/Header.cpp
        src/Pixel.h
        src/ImageProcessing.cpp
        src/ImageProcessing.h
        src/UnitTesting.cpp
        src/UnitTesting.h
        )
