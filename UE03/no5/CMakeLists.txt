cmake_minimum_required(VERSION 3.10)
project(no5 C)

set(CMAKE_C_STANDARD 11)

add_executable(no5 main.c)

target_link_libraries(no5 pthread SDL X11 m)