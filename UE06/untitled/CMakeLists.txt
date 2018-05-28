cmake_minimum_required(VERSION 3.10)
project(make2s C)

set(CMAKE_C_STANDARD 11)

add_executable(make2s main.c)
add_executable(makeXs mainXs.c)