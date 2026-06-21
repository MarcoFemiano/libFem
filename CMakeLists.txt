cmake_minimum_required(VERSION 4.0)
project(libFem C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

function(copy_mingw_runtime_dll target_name dll_name)
    if (MINGW)
        get_filename_component(MINGW_COMPILER_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
        set(MINGW_RUNTIME_DLL "${MINGW_COMPILER_DIR}/${dll_name}")

        if (EXISTS "${MINGW_RUNTIME_DLL}")
            add_custom_command(TARGET ${target_name} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${MINGW_RUNTIME_DLL}"
                    "$<TARGET_FILE_DIR:${target_name}>/${dll_name}")
        endif()
    endif()
endfunction()

add_library(libFem_static STATIC
        stack.c
        coda.c
        BST_AVL.c
        hash_table.c
        graph.c
        LinkedList.c
        third_party/xxHash.c)

set_target_properties(libFem_static PROPERTIES
        OUTPUT_NAME libFem)

target_include_directories(libFem_static PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(libFem_static PUBLIC Threads::Threads)

if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(libFem_static PRIVATE -Wall -Wextra -pedantic)
endif()

add_executable(libFem main.c)
target_link_libraries(libFem PRIVATE libFem_static)
copy_mingw_runtime_dll(libFem libwinpthread-1.dll)

add_executable(libFem_demo_hash EXCLUDE_FROM_ALL mainHashDaCombaciare.c)
target_link_libraries(libFem_demo_hash PRIVATE libFem_static)
copy_mingw_runtime_dll(libFem_demo_hash libwinpthread-1.dll)

add_executable(libFem_demo_parking EXCLUDE_FROM_ALL
        mainEsercitazioneParking.c
        parking.c)
target_link_libraries(libFem_demo_parking PRIVATE libFem_static)
copy_mingw_runtime_dll(libFem_demo_parking libwinpthread-1.dll)

enable_testing()

function(add_libfem_test test_name source_file)
    add_executable(${test_name} ${source_file})
    target_link_libraries(${test_name} PRIVATE libFem_static)
    copy_mingw_runtime_dll(${test_name} libwinpthread-1.dll)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_libfem_test(test_stack tests/test_stack.c)
add_libfem_test(test_coda tests/test_coda.c)
add_libfem_test(test_hash_table tests/test_hash_table.c)
add_libfem_test(test_avl tests/test_avl.c)
add_libfem_test(test_avl_concurrent tests/test_avl_concurrent.c)
add_libfem_test(test_LinkedList tests/test_LinkedList.c)
add_libfem_test(test_graph tests/test_graph.c)
add_libfem_test(test_libfem_header tests/test_libfem_header.c)
