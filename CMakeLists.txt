cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(code
        definition.cpp
        definition.h
        main.cpp)
