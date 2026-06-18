cmake_minimum_required(VERSION 4.0)
project(libFem C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_library(libFem STATIC
        stack.c
        coda.c
        BST_AVL.c
        hash_table.c
        graph.c
        LinkedList.c
        third_party/xxHash.c)

target_include_directories(libFem PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libFem PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(libFem_demo_hash EXCLUDE_FROM_ALL main.c)
target_link_libraries(libFem_demo_hash PRIVATE libFem)

add_executable(libFem_demo_parking EXCLUDE_FROM_ALL
        mainEsercitazioneParking.c
        parking.c)
target_link_libraries(libFem_demo_parking PRIVATE libFem)

enable_testing()

function(add_libfem_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE libFem)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_libfem_test(test_stack tests/test_stack.c)
add_libfem_test(test_coda tests/test_coda.c)
add_libfem_test(test_hash_table tests/test_hash_table.c)
add_libfem_test(test_avl tests/test_avl.c)
add_libfem_test(test_LinkedList tests/test_LinkedList.c)
add_libfem_test(test_graph tests/test_graph.c)
add_libfem_test(test_libfem_header tests/test_libfem_header.c)
