cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)

# PATH TO FindSDL2.cmake
set(CMAKE_MODULE_PATH "D:/SDL2-2.0.9")

# PATH TO SDL
set(SDL2_PATH "D:/SDL2-2.0.9")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

aux_source_directory(. SOURCE_FILES)
add_executable(gamegirl ${SOURCE_FILES})

target_link_libraries(gamegirl ${SDL2_LIBRARY})

