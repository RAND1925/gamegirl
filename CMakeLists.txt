cmake_minimum_required(VERSION 3.12)
project(gamegirl)

set(CMAKE_CXX_STANDARD 17)

# PATH TO FindSDL2.cmake
set(CMAKE_MODULE_PATH "D:/SDL2-2.0.9")

# PATH TO SDL
set(SDL2_PATH "D:/SDL2-2.0.9")

# CMake parses module files by their names. It will use FindSDL2 in this case.
find_package(SDL2 REQUIRED)

# Explicitly specify paths for SDL_gpu. It's hardcoded and ugly but easy.
#set(SDL2_GPU_LIBRARY_PATH "C:/Development/Library/lib/libSDL2_gpu.dll.a")
#set(SDL2_GPU_INCLUDE_PATH "C:/Development/Library/include/SDL2_gpu")

# SDL2_INCLUDE_DIR variable set by the FindSDL2 module (if it finds directory).
#include_directories(${SDL2_INCLUDE_DIR} ${SDL2_GPU_INCLUDE_PATH})
include_directories(${SDL2_INCLUDE_DIR})

aux_source_directory(. SOURCE_FILES)
add_executable(gamegirl ${SOURCE_FILES})
# SDL2MAIN_LIBRARY is also specified within SDL2_LIBRARY by the FindSDL2 module.
# It's needed for Windows specific main function.
# If I don't use FindSDL2 module, I have to link it as well.
target_link_libraries(gamegirl
        #	${SDL2MAIN_LIBRARY}
        ${SDL2_LIBRARY})

