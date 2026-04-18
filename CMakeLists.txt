cmake_minimum_required(VERSION 4.0)
project(libFem C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(libFem
        stack.c
        stack.h
        esercizioValidatoreParentesi.c
        coda.c
        coda.h
        main.c
        parking.c
        parking.h
        BST_AVL.c
        BST_AVL.h)
