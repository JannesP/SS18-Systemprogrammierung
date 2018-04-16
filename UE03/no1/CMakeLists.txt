cmake_minimum_required(VERSION 3.10)
project(no1 C)

set(CMAKE_C_STANDARD 11)

add_executable(no1 main.c)
target_link_libraries(no1 pthread)

add_executable(no2 no2.c)
target_link_libraries(no2 pthread)