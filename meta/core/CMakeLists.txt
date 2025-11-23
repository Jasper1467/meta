# Core is header-only, create an INTERFACE library
add_library(meta_core INTERFACE)

# Include this folder for meta_core
target_include_directories(meta_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

