cmake_minimum_required(VERSION 3.20)
project(code-library-c)

set(CMAKE_C_STANDARD 99)

# $ cc main.c utils.c -o code_library -lreadline
add_executable(${PROJECT_NAME} main.c code-library.h utils.c)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
target_link_libraries(${PROJECT_NAME} readline)
