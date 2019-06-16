cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-O3 -Wall -march=corei7-avx -static)


# PATH TO FindSDL2.cmake
set(CMAKE_MODULE_PATH ${})
# PATH TO SDL
set(SDL2_PATH ${})

add_subdirectory(Catridges)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

aux_source_directory(. SOURCE_FILES)
add_executable(gamegirl ${SOURCE_FILES})

target_link_libraries(gamegirl CartridgesLib ${SDL2_LIBRARY} )

