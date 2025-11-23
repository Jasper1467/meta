add_library(meta_core INTERFACE)
target_include_directories(meta_core INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/..    # Add parent folder of meta/
)

