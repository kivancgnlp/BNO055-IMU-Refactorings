cmake_minimum_required(VERSION 4.0)
project(Project C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Project
        src/bno055.c
        src/bno055.h
        src/Test_main.c)
