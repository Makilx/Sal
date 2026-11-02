# Sal Interface
add_library(
  Sal_Core

  src/IO/File.cpp
  src/Type/Convert.cpp
  src/Task/Task.cpp
  src/Task/TaskScheduler.cpp
)

# Include
target_include_directories(
  Sal_Core
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Precompiled
target_precompile_headers(
  Sal_Core
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include/SalCore/Precompiled.hpp"
)
