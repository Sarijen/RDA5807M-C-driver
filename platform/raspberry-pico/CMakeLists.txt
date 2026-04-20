cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(Pico_RDA5807M C CXX)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_subdirectory(../../ rda5807m)

add_executable(Pico_RDA5807M
  src/main.c
)

target_link_libraries(
  Pico_RDA5807M
  PRIVATE

  pico_stdlib
  hardware_i2c

  RDA5807M
)

pico_add_extra_outputs(Pico_RDA5807M)
