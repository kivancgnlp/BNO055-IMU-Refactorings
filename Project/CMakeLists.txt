cmake_minimum_required(VERSION 4.0)
project(Project C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD  17)

include_directories(src)

add_executable(c_project
        src/bno055.c
        src/bno055.h
        src/Test_main.c)

add_executable(cpp_project new_driver_src/Bno055.cpp new_driver_src/Bno055.h
        new_driver_src/Bno055_MockUp.cpp
        new_driver_src/Bno055_MockUp.h
        new_driver_src/New_Test_Main.cpp
)
