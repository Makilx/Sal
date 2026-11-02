# Sal Interface
add_library(
  Sal_Engine

  src/DataModel.cpp

  src/Services/WindowService.cpp
  src/Services/RunService.cpp
  src/Services/InputService.cpp
  src/Services/Workspace/Workspace.cpp
  src/Services/Workspace/Workspace3D.cpp

  src/Instances/Workspace3D/Part3D.cpp
)

# Include
target_include_directories(
  Sal_Engine
  PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Linking
target_link_libraries(
  Sal_Engine
  PUBLIC
  Sal_Core
  PRIVATE
  Sal_Interface
  Sal_Backend
)
