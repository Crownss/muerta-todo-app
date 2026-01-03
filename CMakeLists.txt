cmake_minimum_required(VERSION 4.1)
project(muerta)

set(CMAKE_CXX_STANDARD 23)

# Enable all warnings and treat them as errors
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
        -Wcast-qual
        -Wcast-align
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wno-unused-parameter
        -fdiagnostics-color=always
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

add_executable(muerta main.cpp)
