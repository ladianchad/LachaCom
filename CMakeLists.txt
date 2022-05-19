cmake_minimum_required(VERSION 3.0.2)
project(communication)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(
  include
)
set(INSTALL_RPATH /usr/lib)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

add_library(communication
  src/serial.cpp
)
target_link_libraries(communication Threads::Threads)

add_executable(test src/sample.cpp)
add_dependencies(test communication)
target_link_libraries(test communication)

install(TARGETS communication
  EXPORT communication
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(DIRECTORY include
  DESTINATION include
)
