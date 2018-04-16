cmake_minimum_required(VERSION 3.10)
project(no3 C)

set(CMAKE_C_STANDARD 11)

add_executable(no3 main.c)

target_link_libraries(no3 pthread SDL X11 m)