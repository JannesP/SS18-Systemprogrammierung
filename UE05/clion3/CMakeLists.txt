cmake_minimum_required(VERSION 3.10)
project(clion3)

set(CMAKE_CXX_STANDARD 11)

add_executable(clion3 main.cpp)
target_link_libraries(clion3 pthread)