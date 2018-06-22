cmake_minimum_required(VERSION 3.9)
project(hMan C)

set(CMAKE_C_STANDARD 11)

add_executable(hMan hMan.c hManLib.h)
target_link_libraries(hMan pthread)