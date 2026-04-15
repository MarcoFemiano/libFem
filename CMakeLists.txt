cmake_minimum_required(VERSION 4.0)
project(libFem C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(libFem
        stack.c
        stack.h
        main.c
        coda.c
        coda.h)
