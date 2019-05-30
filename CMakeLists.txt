cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)
include_directories($ {PROJECT_SOURCE_DIR} /include)
link_directories($ {PROJECT_SOURCE_DIR} /lib)

aux_source_directory(. sourece_files)
add_executable(gamegirl ${sourece_files} Timer.cpp Timer.h)
