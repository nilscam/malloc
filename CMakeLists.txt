cmake_minimum_required(VERSION 3.9)
project(untitled1 C)

set(CMAKE_C_STANDARD 99)

add_library(libmy_malloc SHARED malloc.c malloc.h allocate.c)