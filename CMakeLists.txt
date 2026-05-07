cmake_minimum_required(VERSION 3.10)

project(CHIP-8)

find_package(SDL2 REQUIRED)

add_executable(CHIP-8 src/main.c src/chip8.c)

target_include_directories(CHIP-8 PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(CHIP-8 ${SDL2_LIBRARIES})