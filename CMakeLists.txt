cmake_minimum_required(VERSION 3.5.0)
project(labwork1 VERSION 1.0 LANGUAGES C CXX)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(lib)
add_subdirectory(bin)