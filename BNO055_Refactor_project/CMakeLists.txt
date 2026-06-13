cmake_minimum_required(VERSION 4.0)
project(Project C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

include_directories(src)

add_executable(c_project
        src/bno055.c
        src/bno055.h
        src/Test_main.c)

add_executable(cpp_project new_driver_src/Bno055_Driver.cpp new_driver_src/Bno055_Driver.h
        new_driver_src/New_Test_Main.cpp
        new_driver_src/Bno055_I2C_Interface.cpp
        new_driver_src/Bno055_I2C_Interface.h
        new_driver_src/Bno055_I2C_Mock_Interface.cpp
        new_driver_src/Bno055_I2C_Mock_Interface.h
)
add_executable(BitUtilsTests tests/BitUtils_Tests.cpp)
