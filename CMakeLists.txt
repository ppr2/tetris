cmake_minimum_required(VERSION 3.3)
project(tetris)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
        tetris.c tetris.h stack.c stack.h state.c state.h fitting.c fitting.h
        tests/fitting_test.c tests/fitting_test.h tests/stack_test.c tests/stack_test.h)

add_executable(tetris ${SOURCE_FILES})