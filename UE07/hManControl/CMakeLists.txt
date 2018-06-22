cmake_minimum_required(VERSION 3.9)
project(hManControl C)

set(CMAKE_C_STANDARD 11)

add_executable(hManControl main.c hManLib.h)