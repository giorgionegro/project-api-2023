cmake_minimum_required(VERSION 3.25)
project(api C)
# with O3 optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11  -Wall")
set(CMAKE_C_STANDARD 11)

add_executable(api main.c)
