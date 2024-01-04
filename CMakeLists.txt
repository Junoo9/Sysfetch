cmake_minimum_required(VERSION 3.26)
project(Sysfetch)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")

add_executable(Sysfetch main.cpp)
