cmake_minimum_required(VERSION 3.20)
project(code_library C)

set(CMAKE_C_STANDARD 99)

# $ cc main.c utils.c -o code_library -lreadline
add_executable(code_library main.c code-library.h utils.c)
target_link_libraries(${PROJECT_NAME} readline)
