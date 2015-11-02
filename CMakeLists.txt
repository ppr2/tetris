cmake_minimum_required(VERSION 3.3)
project(tetris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    main.c
    main.h wip/stack.c wip/stack.h stack.c stack.h state.c state.h)

add_executable(tetris ${SOURCE_FILES})