cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(. sourece_files)
add_executable(gamegirl ${sourece_files} Timer.cpp Timer.h)
