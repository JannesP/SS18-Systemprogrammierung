cmake_minimum_required(VERSION 3.10)
project(clion_proj C)

set(CMAKE_C_STANDARD 11)

add_executable(clion_proj no01Vorgabe.c)
target_link_libraries(clion_proj pthread)