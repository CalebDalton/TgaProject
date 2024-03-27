cmake_minimum_required(VERSION 3.27)
project(COP3503)

set(CMAKE_CXX_STANDARD 14)

add_executable(COP3503 cmake-build-debug/mainold.cpp
        cmake-build-debug/ImageData.h
        cmake-build-debug/main.cpp)
