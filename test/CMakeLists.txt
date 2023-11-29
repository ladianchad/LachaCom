cmake_minimum_required(VERSION 3.0.2)
project(test_com)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(LaChaCom REQUIRED)

include_directories(
  ${LaChaCom_INCLUDE_DIR}
)

add_executable(test main.cpp)
target_link_libraries(test LaChaCom::LaChaCom)
