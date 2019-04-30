cmake_minimum_required(VERSION 3.12)
project(CPPGB)

set(CMAKE_CXX_STANDARD 17)

add_executable(CPPGB main.cpp CPU.cpp CPU.h common.h Op.cpp Op.h Memory.h MMU.cpp MMU.h)