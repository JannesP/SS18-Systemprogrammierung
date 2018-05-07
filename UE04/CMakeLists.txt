cmake_minimum_required(VERSION 3.10)
project(ring C)

set(CMAKE_C_STANDARD 11)

add_executable(ring no01Vorgabe.c)
target_link_libraries(ring pthread)