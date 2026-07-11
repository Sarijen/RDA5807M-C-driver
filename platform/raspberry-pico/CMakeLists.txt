cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(Pico_RDA5807M C CXX)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_subdirectory(../../ rda5807m)


file(GLOB EXAMPLE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.c"
)

foreach(src ${EXAMPLE_SOURCES})
  get_filename_component(example_name "${src}" NAME_WE)

  add_executable(${example_name} "${src}")

  set_target_properties(${example_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples/${example_name}
  )

  target_link_libraries(
    ${example_name} 
    PRIVATE

    pico_stdlib
    hardware_i2c

    RDA5807M
  )

  pico_enable_stdio_uart(${example_name} 0)
  pico_enable_stdio_usb(${example_name} 1)

  pico_add_extra_outputs(${example_name})
endforeach()


add_executable(hello examples/base.c)
  target_link_libraries(
    hello
    PRIVATE

    pico_stdlib
    hardware_i2c

    RDA5807M
  )
set_target_properties(hello PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${example}"
)
get_target_property(dir hello RUNTIME_OUTPUT_DIRECTORY)
message(STATUS "hello output = ${dir}")
