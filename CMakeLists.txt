cmake_minimum_required(VERSION 3.0.2)
project(LaChaCom)

if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum-0.9.5 AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum-0.9.5/CMakeLists.txt)
  message(STATUS "magic_enum-0.9.5 detected...")
else()
  message(STATUS "GET MAGIC ENUM...")
  execute_process(
    COMMAND wget -c https://github.com/Neargye/magic_enum/archive/refs/tags/v0.9.5.tar.gz -O  magic_enum-0.9.5.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}  
  )
  execute_process(
    COMMAND tar -xzvf magic_enum-0.9.5.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  execute_process(
    COMMAND rm -rf magic_enum-0.9.5.tar.gz
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
endif()


set(MAGIC_ENUM_OPT_INSTALL TRUE)
add_subdirectory(magic_enum-0.9.5)

find_package(nlohmann_json REQUIRED)
find_package(Threads REQUIRED)
find_package(spdlog REQUIRED)


include_directories(
  include
  ${SPDLOG_INCLUDE_DIR}
)

set(library_name ${PROJECT_NAME})

add_library(${library_name}
  src/interface.cpp
  interfaces/uart.cpp
)
add_library(${PROJECT_NAME}::${library_name} ALIAS ${library_name})
target_link_libraries(${library_name} PRIVATE  magic_enum::magic_enum spdlog::spdlog_header_only nlohmann_json::nlohmann_json Threads::Threads)

add_executable(test src/main.cpp)
add_dependencies(test ${library_name})
target_link_libraries(test ${library_name})

include(GNUInstallDirs)

install(TARGETS ${library_name} 
  EXPORT  ${PROJECT_NAME}Targets
  LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}                     
  RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}                       
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  NAMESPACE ${PROJECT_NAME}::
  DESTINATION lib/cmake/${PROJECT_NAME}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  INSTALL_DESTINATION lib/cmake/${PROJECT_NAME}
)

export(PACKAGE ${PROJECT_NAME})

install(DIRECTORY include/communication
  DESTINATION include
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION lib/cmake/${PROJECT_NAME}
)