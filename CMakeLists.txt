# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(AVL)

# Add the executable target
add_executable(${PROJECT_NAME} main.cpp avl.hpp avl.cxx)
