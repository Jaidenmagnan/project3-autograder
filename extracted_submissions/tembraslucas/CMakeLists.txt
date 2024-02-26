cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(image-processor)
add_executable(image-processor src/main.cpp src/Pixel.cpp)