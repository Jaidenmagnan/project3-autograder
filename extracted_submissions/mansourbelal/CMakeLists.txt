cmake_minimum_required(VERSION 3.10) # Specify the minimum version you require.

project(image-processor) # Name your project.

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# Define an executable target named 'TGAImageApp' and specify its source files.
add_executable(image-processor src/another1.cpp
)
