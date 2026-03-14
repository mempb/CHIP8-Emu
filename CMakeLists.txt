cmake_minimum_required(VERSION 3.10)

project(CHIP-8)

set(SDL2_DIR "C:/Dev/msys64/ucrt64/lib/cmake/SDL2")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(CHIP-8 src/main.c src/chip8.c)

link_directories("C:/Dev/msys64/ucrt64/lib")
target_link_libraries(CHIP-8 
    "C:/Dev/msys64/ucrt64/lib/libSDL2.a"
    -lsetupapi
    -limm32
    -lwinmm
    -lcfgmgr32
    -lversion
)