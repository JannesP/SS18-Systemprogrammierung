cmake_minimum_required(VERSION 3.10)
project(showPhilos C)

set(CMAKE_C_STANDARD 11)

add_executable(showPhilosTest showPhilosTest.c)
target_link_libraries(showPhilosTest pthread)

add_executable(showPhilos showPhilos.c)
target_link_libraries(showPhilos GL GLU glut pthread m)

add_executable(philos philos.c)
target_link_libraries(philos pthread)

