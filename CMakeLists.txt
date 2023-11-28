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

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum-0.8.0 AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum-0.8.0/CMakeLists.txt)
  message(STATUS "magic_enum-0.8.0 detected...")
else()
  message(STATUS "GET MAGIC ENUM...")
  execute_process(
    COMMAND wget -c https://github.com/Neargye/magic_enum/archive/refs/tags/v0.8.0.tar.gz -O  magic_enum-0.8.0.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
  )
  execute_process(
    COMMAND tar -xzvf magic_enum-0.8.0.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  execute_process(
    COMMAND rm -rf magic_enum-0.8.0.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()

add_subdirectory(magic_enum-0.8.0)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(
  include
)

add_library(communication
  src/interface.cpp
)
target_link_libraries(communication Threads::Threads nlohmann_json::nlohmann_json magic_enum::magic_enum)

add_executable(test src/main.cpp)
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
