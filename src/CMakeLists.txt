cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O3 -Wall -march=corei7-avx -static)
add_subdirectory(Catridges)

# PATH TO FindSDL2.cmake
#set(CMAKE_MODULE_PATH "path_to_FindSDL2.camke")
# PATH TO SDL
#set(SDL2_PATH "path_to_sdl2")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../bin/")

aux_source_directory(. SOURCE_FILES)
add_executable(gamegirl ${SOURCE_FILES})

target_link_libraries(gamegirl libCartridges ${SDL2_LIBRARY} )

