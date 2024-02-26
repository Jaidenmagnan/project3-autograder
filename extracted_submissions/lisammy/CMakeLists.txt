cmake_minimum_required(VERSION 3.26)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)

add_executable(image-processor src/main.cpp
        src/Image.h
        src/pixelData.h
        src/Image.cpp
        src/blends.h
        src/outFile.h
        src/checker.h
)
