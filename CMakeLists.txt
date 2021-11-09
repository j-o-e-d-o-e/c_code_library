cmake_minimum_required(VERSION 3.20)
project(code_library C)

set(CMAKE_C_STANDARD 99)

# $ cc main.c utils.c -o code_library
add_executable(code_library main.c code-library.h utils.c)
