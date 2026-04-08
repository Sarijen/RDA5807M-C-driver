cmake_minimum_required(VERSION 3.18)

project(RDA5807M C CXX)
set(CMAKE_C_STANDARD 11)

add_library(RDA5807M STATIC src/RDA5807M.c)

target_include_directories(
  RDA5807M PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
