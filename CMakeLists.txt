cmake_minimum_required(VERSION 3.15.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Sal Engine Project
project(
  Sal
  VERSION 1.0
  LANGUAGES C CXX
)

add_library(Sal src/Dummy.cpp)

add_subdirectory(src/core)
add_subdirectory(src/interface)
add_subdirectory(src/backend)
add_subdirectory(src/engine)

target_link_libraries(
  Sal
  PUBLIC
  Sal_Engine
  Sal_Core
  PRIVATE
  Sal_Interface
  Sal_Backend
)

# Resources
add_custom_target(SalResources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "${CMAKE_BINARY_DIR}/resources"
    COMMENT "Updating Sal resources..."
)

# Debug
add_executable(Sal_Debug src/Dummy.cpp)
target_link_libraries(
  Sal_Debug
  PRIVATE
  Sal_Engine
  Sal_Core
)
