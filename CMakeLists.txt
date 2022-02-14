cmake_minimum_required(VERSION 3.0.2)
project(communication)

include_directories(
  include
)

add_library(communication
  src/serial.cpp
)

add_executable(test src/test.cpp)
add_dependencies(test communication)
target_link_libraries(test communication)
