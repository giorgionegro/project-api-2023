cmake_minimum_required(VERSION 3.25)
project(api C)
# with O3 optimization
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11  -march=native -mtune=native -Ofast -Wall -Wpedantic -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-but-set-parameter -Wno-unused-value -Wno-unused-result")
set(CMAKE_C_STANDARD 11)

add_executable(api main.c)
