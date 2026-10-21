# Sal Interface
add_library(Sal_Interface INTERFACE)

# Include
target_include_directories(
  Sal_Interface
  INTERFACE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Linking
target_link_libraries(
  Sal_Interface
  INTERFACE
  Sal_Core
)
