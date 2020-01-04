cmake_minimum_required(VERSION 3.10)
project(hash_table VERSION 0.0.1)

set(CMAKE_C_FLAGS "-Wall")

add_executable(hash_table
  main.c
  hash_table.h
  hash_table.c
  prime.h
  prime.c)

# Need to link the cmath library (m)
target_link_libraries(hash_table m)