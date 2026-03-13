cmake_minimum_required(VERSION 3.10)

project(CHIP-8)

set(SDL2_DIR "C:/SDL2/SDL2-2.30.3/x86_64-w64-mingw32/lib/cmake/SDL2")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

add_executable(chip8 src/main.c src/chip8.c)
target_include_directories(chip8 PRIVATE inc/ ${SDL2_INCLUDE_DIRS})
link_directories("C:/SDL2/SDL2-2.30.3/x86_64-w64-mingw32/lib")
target_link_libraries(chip8 
    "C:/SDL2/SDL2-2.30.3/x86_64-w64-mingw32/lib/libSDL2.a"
    -lsetupapi
    -limm32
    -lwinmm
    -lcfgmgr32
    -lversion
)