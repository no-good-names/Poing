cmake_minimum_required(VERSION 3.28)
project(Poing C)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_C_STANDARD 99)

add_executable(Poing main.c
        font.c
        font.h)

target_link_libraries(Poing ${SDL2_LIBRARIES})
